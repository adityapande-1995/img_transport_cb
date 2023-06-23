# img_transport_cb
This temporary repo is used to demonstrate the use of callback groups with image transport.

# To build
Source your ros2 rolling distribution, install dependencies of this package using rosdep, and run : 
```
mkdir build
cd build
cmake ..; make
```

# To run
Download any test image and place it at the root as say `some_image.jpeg`
In terminal 1, run :
```
./build/my_publisher some_image.jpeg
```

In terminal 2 :
```
./build/my_subscriber
```

Work in progress :
This should run the 2 callbacks that are mutually exclusive in the selected callback queue. By default these are mutually exclusive, so the sleep() in
the second callback blocks the execution of the original callback.

Changing the group type to reentrant and changing the executor to multithreaded should fix the issue. (Need to test this)

