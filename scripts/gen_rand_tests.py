import random
import sys


if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: num_tests num_addr output_file_name")
        exit(1)

    file = open(sys.argv[3], "w")
    for i in range(0, int(sys.argv[2])):
        addr = random.randint(0, int(sys.argv[1]))
        itype = 's' if random.randint(0, 1) == 0 else 'l'
        to_write = f'{itype} {hex(addr)} 0\n'
        # print(to_write)  
        file.write(to_write)
        


