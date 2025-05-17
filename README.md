# Binary Search Tree Console Visualization

## Why It Was Developed

```
                     ┏━━━┓                     
           ┏━━━━━━━━━┫ 5 ┣━━━━━━━━━┓           
           ┃         ┗━━━┛         ┃           
         ┏━┻━┓                   ┏━┻━┓         
     ┏━━━┫ 2 ┣━━━┓           ┏━━━┫11 ┣━━━┓     
     ┃   ┗━━━┛   ┃           ┃   ┗━━━┛   ┃     
   ┏━┻━┓       ┏━┻━┓       ┏━┻━┓       ┏━┻━┓   
   ┃ 1 ┃       ┃ 3 ┃       ┃ 7 ┃       ┃13 ┃   
   ┗━━━┛       ┗━━━┛       ┗━━━┛       ┗━━━┛
```
This is Final Project of Programming Method & Abstraction course.

Getting inspired by visualization websites, this project is developed to
visualize Binary Tree on console UI in text-based presentation. Under this form, Binary Tree can be shared straightforwardly in text communication like technical documents, code reviews, or source code documentation, all without the need of images.

Currently the program demonstrates how AVL Tree changes through out insertion and deletion. The output is made with ASCII characters, and can be exported to a text file for sharing.

## Architecture Diagram
```
╭┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄╮
┆                      Main program module  ┆
┆       ┏━━━━━━━━━━━━━━━━━━━┓               ┆ 
┆    ┏━━┫ Program Interface ┣━━┓            ┆
┆    ┃  ┗━━━━━━━━━━━━━━━━━━━┛  ┃            ┆
┆┄┄┄┄┃┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┃┄┄┄┄┄┄┄┄┄┄┄┄┆
┆ ┏━━┻━━┓               ┏━━━━━━┻━━━━━━┓     ┆
┆ ┃ BST ┃               ┃ BST Drawing ┃     ┆
┆ ┗━━━━━┛               ┗━━━━━━━━━━━━━┛     ┆
┆                                           ┆
┆               Tree implementation module  ┆ 
╰┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄╯
```

## How to Compile and Run
   
1. **Compile the Code**:
   ```bash
   ./build.sh
   ```
   Output dir: `build`

1. **Test the Core Logic**:
   ```bash
   ./test.sh
   ```

## Technologies Used

| **Technology**         | **Description**                                   |
|-------------------------|---------------------------------------------------|
| **Programming Language**| C                                             |
| **Testing Framework**   | Google Test                     |

## Features & Future Enhancements

| Feature / Enhancement                                      | Status      |
|------------------------------------------------------------|-------------|
| Visualize Binary Search Tree (BST) in console UI           | ✅  |
| ASCII-based tree drawing                                   | ✅  |
| AVL Tree insertion and deletion visualization              | ✅  |
| Export output to text file                                 | ✅  |
| Read tree content from file                                | ✅  |
| Export to various content types                            | ⬜  |
| Implement different BST balancing methods                  | ⬜  |
| Step-by-step tree changes for each operation               | ⬜  |
| Multiple drawing styles for nodes and connections          | ⬜  |
| Support for non-monospaced fonts                           | ⬜  |
| Optimize drawing buffer memory                             | ⬜  |
