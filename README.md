# Binary Search Tree Console Visualization

```
               ___               
              |   |              
        ______| 5 |______        
       |      |___|      |       
       |                 |       
      _|_              __|_      
     |   |            |    |     
   __| 2 |__        __| 11 |__   
  |  |___|  |      |  |____|  |  
  |         |      |          |  
 _|_       _|_    _|_       __|_ 
|   |     |   |  |   |     |    |
| 1 |     | 3 |  | 7 |     | 13 |
|___|     |___|  |___|     |____|
```

## Why

Getting inspired by visualization websites, this project is developed to
visualize Binary Tree on console UI in text-based presentation. Under this form, Binary Tree can be shared straightforwardly in text communication like technical documents, code reviews, or source code documentation, all without the need of images.

Currently the program demonstrates how AVL Tree changes through out insertion and deletion. The output is made with ASCII characters, and can be exported to or imported from a text file for sharing.

## Project Diagram
```
╭┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄╮
┆                      Main program module  ┆
┆       ┏━━━━━━━━━━━━━━━━━━━┓               ┆ 
┆    ┏━━┫ Program Interface ┣━━┓            ┆
┆    ┃  ┗━━━━━━━━━━━━━━━━━━━┛  ┃            ┆
┆┄┄┄┄┃┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┃┄┄┄┄┄┄┄┄┄┄┄┄┆
┆ ┏━━┻━━━━━━━━┓         ┏━━━━━━┻━━━━━━━┓    ┆
┆ ┃ BST Impl. ┃         ┃ BST Printing ┃    ┆
┆ ┗━━━━━━━━━━━┛         ┗━━━━━━━━━━━━━━┛    ┆
┆                                           ┆
┆               Tree implementation modules ┆ 
╰┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄╯
```

## How to Compile and Run
   
1. **Compile the code**:
   ```bash
   ./build.sh           # Debug
   ./build.sh release   # Release
   ```
   Output directory: `build`

1. **Verify all test cases**:
   ```bash
   ./test.sh
   ```
   (Requires libgtest, libfmt.)

## Technologies Used

| **Description**         | **Technology**                                   |
|-------------------------|---------------------------------------------------|
| **Language**| C, C++, Bash script                                             |
| **Testing Framework**   | Google Test                     |

## Features & Future Enhancements

| Feature / Enhancement                                      | Status      |
|------------------------------------------------------------|-------------|
| Visualize Binary Search Tree (BST) in console UI           | ✅  |
| ASCII-based tree printing                                   | ✅  |
| AVL Tree insertion and deletion visualization              | ✅  |
| Export output to text file                                 | ✅  |
| Read tree content from exported file                                | ✅  |
| Export to various content types                            | ⬜  |
| Implement different BST balancing methods                  | ⬜  |
| Step-by-step tree changes for each operation               | ⬜  |
| Multiple drawing styles for nodes and connections          | ⬜  |
| Support for non-monospaced fonts                           | ⬜  |
| Optimize drawing buffer memory                             | ⬜  |

**Modifications are welcome!**