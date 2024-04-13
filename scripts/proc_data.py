import pandas as pd
import pickle
import matplotlib.pyplot as plt
from data import *
import math

bindata = open('bin_data', 'rb')
results = pickle.load(bindata)

actual_sizes = []
for mapping in mappings:
    sets, assoc, bytes_per_block = mapping
    # actual size
    s = math.log2(sets) 
    m = math.log2(bytes_per_block) - 2 
    b = math.log2(assoc)

    actual_size = (2**s) * (2**b) * ((32 - s - m - b - 2) + 1 + 32 * (2 ** m))
    actual_sizes.append(actual_size)
    # print("size: ", sets* assoc * bytes_per_block )
    # print("actual_size: ", actual_size)

df = pd.DataFrame.from_dict(results, orient='index')
df['hit_rate'] = df.apply(lambda x: (x['load_hits'] + x['store_hits']) / (x['load_hits'] + x['load_misses'] + x['store_hits'] + x['store_misses']), axis=1)

df['actual_size'] = actual_sizes

# -------------------- For Assoc Check --------------------
new_indices = []
for name in df.index:
    name = name.split("_")[2]
    new_indices.append(name)


# df['block_size'] = []

df.index = new_indices
plottables = ['total_cycles']

print(df)
for plot in plottables:
    title = f"cache_size_{plot}"
    df.plot(y=[plot], x='actual_size', kind='line', title=title)
    plt.tight_layout()
    #plt.savefig(f"../graphs/{title}.png")
plt.show()

