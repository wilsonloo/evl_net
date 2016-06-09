# evl_logger
C++ 使用的log，里面使用了log4cplus 和 终端颜色输出

### 依赖
  utility 套件（私有）：https://github.com/wilsonloo/utility.git
  evl_logger ： https://github.com/wilsonloo/evl_logger.git

### 使用方式
  1. git clone https://github.com/wilsonloo/evl_net.git
  2. cd evl_net;mkdir build;cd build
  3. cmake .. 
      注意：本套件使用 PF_WINDOWN 和 _PF_LINUX 用以区分平台，可修改CMakeLists.txt
  4. make
  5. sudo make install 
      注意：默认安装目录为 /usr/local/<source><lib>/evl_net
