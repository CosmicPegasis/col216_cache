import sys
import subprocess
import pickle
import pandas as pd
total_cache_size = 16384
from data import *

if len(sys.argv) != 3:
    print("usage: python3 best_cache_tester.py tracefile outputfile")
    exit(1)

tracefile = sys.argv[1]
outputfile = sys.argv[2]

with open(tracefile, 'r') as f:
    trace_data = f.read()

outputf = open(outputfile, 'w')
results = {}
for mapping in mappings:
    for alloc_type in alloc_types:
        for write_type in write_types:
            for evictor_type in evictor_types:
                cache_size, associativity, block_size = mapping
                command = f"./cacheSim {cache_size} {associativity} {block_size} {alloc_type} {write_type} {evictor_type}"
                           
                print(command)
                outputf.write(command + "\n")
                process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
                stdout, stderr = process.communicate(input=trace_data.encode())
                output = stdout.decode() 
                outputf.write(output + "\n")
                print(output)
                # Parse the output
                output = output.strip().split('\n')
                result = {
                    'total_loads': int(output[0].split(':')[1].strip()),
                    'total_stores': int(output[1].split(':')[1].strip()),
                    'load_hits': int(output[2].split(':')[1].strip()),
                    'load_misses': int(output[3].split(':')[1].strip()),
                    'store_hits': int(output[4].split(':')[1].strip()),
                    'store_misses': int(output[5].split(':')[1].strip()),
                    'total_cycles': int(output[6].split(':')[1].strip()),
                    'cache_size': cache_size,
                    'associativity': associativity,
                    'block_size': block_size,
                    'alloc_type': alloc_type,
                    'write_type': write_type,
                    'evictor_type': evictor_type,
                }
                key = f"{cache_size}_{associativity}_{block_size}_{alloc_type}_{write_type}_{evictor_type}"
                results[key] = result

# open a file named bin data then pickle results and save it in the file
bindata = open('bin_data', 'wb')
# dump results into a csv using pandas
pd.DataFrame.from_dict(results, orient='index').to_csv("data.csv")
pickle.dump(results, bindata)
# setup a pandas dataframe
