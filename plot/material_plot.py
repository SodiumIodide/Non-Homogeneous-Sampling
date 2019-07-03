#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

def main():
    '''Main function'''
    data_exists = True
    try:
        data = pd.read_csv("../csv/material_plot.csv")
    except FileNotFoundError:
        data_exists = False
    if data_exists:
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
        for df in dfs:
            linecolor = 'b'
            if (df['material'].values[0] == 1):
                linecolor='r'
            plt.plot(df['xdist'], df['material'], color=linecolor)

        # Fill out image data
        plt.grid(b=True, which="both", axis="x")
        plt.title("Material Distribution")
        plt.xlabel("x (cm)")
        plt.ylabel("Material Number (integers)")
        yint = range(0, 2)
        plt.yticks(yint)

        # Save image
        plt.savefig("../img/material_plot.png")
        plt.cla()
        plt.clf()

if __name__ == '__main__':
    main()
