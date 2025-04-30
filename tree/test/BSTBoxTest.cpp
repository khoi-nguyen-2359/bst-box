#include <gtest/gtest.h>
#include <sstream>

#include "AVL.h"
#include "BSTBox.h"

using std::stringstream;

class BSTBoxTest : public ::testing::Test {
    protected:
        AVLNode* avl = nullptr;
        BSTBox* box = nullptr;

        void TearDown() override {
            deleteAVLNode(avl);
            deleteBSTBox(box);
        }
};

TEST_F(BSTBoxTest, Present) {
    avl = createAVLNode(1);
    insertAVLNode(avl, 2);
    insertAVLNode(avl, 3);

    box = createBSTBox(avl);
    stringstream os;

    presentBSTBox(os, box);

    string expectedOutput = 
R"(      ___      
     |   |     
   __| 2 |__   
  |  |___|  |  
  |         |  
 _|_       _|_ 
|   |     |   |
| 1 |     | 3 |
|___|     |___|
               
)";

    EXPECT_EQ(os.str(), expectedOutput);
}

TEST_F(BSTBoxTest, PresentLargeValues) {
    avl = createAVLNode(1);
    insertAVLNode(avl, 1000000000);
    insertAVLNode(avl, 1000000001);
    insertAVLNode(avl, -100000000);

    box = createBSTBox(avl);
    stringstream os;

    presentBSTBox(os, box);

    string expectedOutput = 
R"(                ____________          
               |            |         
             __| 1000000000 |__       
            |  |____________|  |      
            |                  |      
           _|_           ______|_____ 
          |   |         |            |
        __| 1 |         | 1000000001 |
       |  |___|         |____________|
       |                              
 ______|_____                         
|            |                        
| -100000000 |                        
|____________|                        
                                      
)";

    EXPECT_EQ(os.str(), expectedOutput);
}