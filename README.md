# Running the Code

To execute the code, follow these steps:

1. **Navigate to the Source Directory**

   Change to the `src` directory by running the following command:

   ```bash
   cd src
   ```

2. **Clean and Compile the Code**

   Clean any previous builds and compile the code by executing:

   ```bash
   make clean
   make
   ```

3. **Run the Executable**

   After compiling, run the executable named `matrix_multiplication`:

   ```bash
   ./matrix_multiplication
   ```

4. **Input Parameters**

   You will be prompted to enter the number of rows and columns for Matrix A, followed by the sparsity level. Repeat this process for Matrix B.
   example in put 10 10 for a 10 by 10 matrix

5. **Choose an Operation**

   After entering the matrix parameters, you will be asked to select the operation you would like to perform.

6. **Optimization Options**

   You will then have the option to apply optimizations, which will execute the selected operation and provide a time indicator for performance.

7. **Performance Testing**

   Following this, you will be prompted to decide whether you want to run a performance test based on your chosen optimization.

8. **Experimental Results**

   You will have the option to view experimental results, which will yield data similar to what was collected during testing. This will showcase the operations that contributed to the results.

9. **Optimization Experiment**

   If desired, you can explore the optimization experiment, which will perform similar tests and introduce an experimental mode for multi-threading. This mode allows you to specify the number of threads for execution.
