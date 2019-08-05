#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from matplotlib.lines import Line2D
custom_lines = [Line2D([0], [0], color='b', lw=4),
                Line2D([0], [0], color='r', lw=4)]

def main():
    '''Main function'''
    data_exists = True
    try:
        data = pd.read_csv("../csv/material_plot.csv")
    except FileNotFoundError:
        data_exists = False
    if data_exists:
        # Total width
        tot_width = data['xdist'].iloc[-1] + data['xdelta'].iloc[-1]
        num_cells = 0
        # Locate number of cells (note: zero-indexing in Python)
        not_found = True
        split_index = 0
        index = 0
        comp_val = data['material'][index]
        while not_found:
            index += 1
            curr_val = data['material'][index]
            if index == len(data)-1:
                not_found = False
                split_index = index + 1
            if curr_val != comp_val:
                not_found = False
                split_index = index

        # Split dataframe
        dfs = [data.loc[i:i+split_index-1,:] for i in range(0, len(data), split_index)]

        # Plot each line of materials
        fig, ax = plt.subplots(1)
        fig.set_figheight(2)
        fig.set_figwidth(7)
        colors = np.array([])
        heights = np.array([])
        xs = np.array([])
        widths = np.array([])
        for df in dfs:
            linecolor = 'b'
            if (df['material'].values[0] == 1):
                linecolor='r'
            for x in df['xdist']:
                num_cells += 1
                colors = np.append(colors, linecolor)
                heights = np.append(heights, 1.0)
                xs = np.append(xs, x)
            for width in df['xdelta']:
                widths = np.append(widths, width)
            #plt.plot(df['xdist'], df['material'], color=linecolor)
        ax.bar(xs, heights, width=widths, color=colors, align='edge', linewidth=min(1, 1.0 / (1.0 * num_cells) * 15))
        plt.xlim([0.0, tot_width])
        plt.ylim([0.0, 1.0])
        ax.set_yticklabels([])
        ax.get_yaxis().set_visible(False)
        ax.legend(custom_lines, ['Material 1', 'Material 2'], loc='upper right', framealpha=1.0, bbox_to_anchor=(1.03, 1.2))

        fig.subplots_adjust(bottom=0.25, top=0.85, left=0.05, right=0.95)

        # Fill out image data
        plt.title("Material Distribution")
        plt.xlabel("x (cm)")

        # Save image
        plt.savefig("../img/material_plot.png")
        #plt.show()
        plt.cla()
        plt.clf()

if __name__ == '__main__':
    main()
