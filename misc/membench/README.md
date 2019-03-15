Measured and graphed using https://github.com/KDE/heaptrack and [membench.cpp](../../bench/membench.cpp)

#### Advantages of manual memory allocation

The auto allocation strategy performs hundreds of thousands of allocations:

![auto-alloc](./auto-alloc.png)

Manual allocation, as expected, performs less allocations by several orders of magnitude:

![manual-alloc](./manual-alloc.png)
