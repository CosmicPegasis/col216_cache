# Direct Mapped, Set Assoc, Fully Assoc
#mappings = [(1024, 1, 16)]
#mappings = [(1, 2048, 16)]
mappings = []
for i in range(4,15):
    mappings.append((1, 2**i, 4))    
## print(mappings)
alloc_types = ['write-allocate']
write_types = ['write-back']
evictor_types = ['lru']
