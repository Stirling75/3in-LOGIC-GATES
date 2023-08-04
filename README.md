# 3in-LOGIC-GATES

This implementation is based on [TFHE library](https://github.com/tfhe/tfhe), and thus TFHE must be installed to run our code :) 

To install TFHE, 



> git clone --recurse-submodules --branch=master https://github.com/tfhe/tfhe.git
> 
> cd tfhe
> 
> mkdir build
> 
> cd build
> 
> cmake ../src
> 
> make
> 
> make install


Also, include



> TFHE_PREFIX=/usr/local #the prefix where you installed tfhe
> 
> export C_INCLUDE_PATH=$C_INCLUDE_PATH:$TFHE_PREFIX/include
> 
> export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:$TFHE_PREFIX/include
> 
> export LIBRARY_PATH=$LIBRARY_PATH:$TFHE_PREFIX/lib
> 
> export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$TFHE_PREFIX/lib
> 


this to your .bashrc file!

Then run 


 > run runcomp.sh


to run our implementation code :)

