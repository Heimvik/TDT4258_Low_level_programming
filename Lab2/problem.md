# The problem
- Look at the pasted images, the problem is that main starts with allocating 2 words for the 3 variables i need to push to the stack in drawBlock
- We pop this variables off the stack in the drawblock funtion, do after the first run, everython is ok.
- However, after the first run, we never proceed to move the stack pointer to a lower location, as the compiler puts this
  over the while loop for some reason leaving the stack pointer to inccrementally go 0x8 farther up than it should each run, +0x0 wrong after the zeroth run, +0x8 wrong the first run, +0xF wrong the second, etc.
What can be done about this? Why is not the compiler allocationg 8 new for each loop run?