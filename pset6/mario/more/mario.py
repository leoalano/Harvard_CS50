import cs50

# prompt user for height of pyramid
while True:
    height = cs50.get_int("Input height of pyramid (between 1 and 8) ")
    if height > 0 and height < 9:
        break

# print pyramid accordingly

# major loop
for i in range(1, height + 1):
    # spaces on left
    for x in range(height - i):
        print(' ', end='')
        
    # hashes on left    
    for x in range(i):
        print('#', end='')
        
    # spaces in the middle    
    print('  ', end='')
    
    # hashes on right
    for x in range(i):
        print('#', end='')
        
    # new line    
    print('')
