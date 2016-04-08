# algorithm_in_c 

C语言的数据结构和算法实现

代码中几乎每一行都添加了注释，通过这种甚至有点啰嗦的方式，希望让人们能够容易的掌握数据结构和算法

如果对数据结构和算法有热情或者其他疑问，欢迎加入QQ群: 530133597

## TODO

### 排序
- [x] 冒泡排序
- [x] 直接插入排序
- [x] 希尔排序
- [x] 二分查找插入排序
- [x] 选择排序
- [x] 快速排序
- [x] 归并排序
- [ ] 堆排序

### 查找
- [x] 二分查找

### 数据结构
- [x] 单向链表
- [x] 双向链表
- [x] 单向循环链表
- [x] 双向循环链表
- [x] 栈 - 通过数组实现
- [x] 栈 - 通过双向链表实现
- [x] 队列 - 使用双向链表实现
- [x] 优先级队列 - 使用双向链表实现
- [ ] 优先级队列 - 使用堆实现
- [ ] 哈希 - hash,也称为散列
- [ ] 堆
- [ ] 树


## 使用方法:

> 进入各个目录执行make即可编译文件，之后运行文件即可


## sort 排序

### BubbleSort 冒泡排序
### InsertSort - Direct Insert Sort 插入排序(直接插入排序)
### ShellSort 希尔排序
### BinInsertSort 二分查找插入排序

> 直接插入排序对于大部分有序的序列排序时速度快。<br/>
> 二分插入排序在直接插入的基本上改变了查找元素插入位置的方法，对于完全无序的序列来说，速度会变快，但是对开大部分有序的序列反而会更慢。<br/>
> 希尔排序则利用直接插入排序对于大部分有序的序列速度快的特点，先让大部分序列有序，以此来提高排序效率。<br/>

### SelectSort 选择排序
### QuickSort 快速排序
### MergeSort 归并排序

## search 查找

### BinarySearch 二分查找

## data_structure 数据结构

### SingleLinkedList 单向链表
### DoubleLinkedList 双向链表
### SingleCircularLinkedList 单向循环链表
### DoubleCircularLinkedList 双向循环链表
### StackByArr 通过数组实现的栈
### StackByDoubleLinkedList 通过双向链表实现的栈
