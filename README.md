# neural-network-c
Artificial Neural Network (ANN) playground made in C, focusing on feedforward neural networks. All matrix operations and training processes are implemented from scratch. Note that this project is mainly created for learning purposes.

## Includes
The project consists of a neural network library and two examples built on top of that.
* **/src**: Contains source files for matrix operations and neural network management. These files later compiled into a library. The library was designed to support arbitrary network shapes and activation/loss functions.
* **/examples**: Includes two example applications, [function-approximator](examples/function-approximator/) and [mnist-learner](examples/mnist-learner/).

## Examples
### Function Approximator
This example is inspired by the Universal Approximation Theorem. In this case the function is in [function-approximator/plotter.h](examples/function-approximator/plotter.h) as **FunctionToLearn()**. Learning process of the network is visualized with [Raylib](https://github.com/raysan5/raylib), using it as a function plotter. Model parameters can be adjusted in [function-approximator/model.c](examples/function-approximator/model.c).
<p align="center">
  <img src="/assets/function_example_0.gif" width="48%" />
  <img src="/assets/function_example_1.gif" width="48%" />
  <img src="/assets/function_example_2.gif" width="48%" />
</p>

### MNIST Learner
This example is aiming training the network to identify images of handwritten numbers. The network is trained using MNIST dataset, a well-known dataset of handwritten images with 60K training samples and 10K test samples. Used [Raylib](https://github.com/raysan5/raylib) to create a simple drawer application to make the network interactable. There is a [pre-trained model](examples/mnist-learner/model/model.nn) with a %97.16 test accuracy, trained with default configurations in [mnist-learner/model.c](examples/mnist-learner/model.c). The library supports saving models in binary format.
<p align="left">
  <img src="/assets/mnist.gif" width="50%" />
</p>

## How to Compile
CMake is configured to compile the library and two examples at once. Raylib and MNIST dataset also will be fetched by CMake. Use this commands in the root directory of the repo:
```
git clone https://github.com/eldemirberkay0/neural-network-c.git
cd neural-network-c
mkdir build
cd build
cmake ..
cmake --build .
```
This will compile two examples into `build/examples`.