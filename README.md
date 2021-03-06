# evl_logger
C++ 使用的log，里面使用了log4cplus 和 终端颜色输出

### 依赖
  1. boost
  2. log4cplus
  3. utility 套件（私有）：https://github.com/wilsonloo/utility.git
  4. evl_logger ： https://github.com/wilsonloo/evl_logger.git

### 使用方式
  1. git clone https://github.com/wilsonloo/evl_net.git
  2. cd evl_net;mkdir build;cd build
  3. cmake .. 
      注意：本套件使用 PF_WINDOWN 和 _PF_LINUX 用以区分平台，可修改CMakeLists.txt
  4. make
  5. sudo make install 
      注意：默认安装目录为 /usr/local/<include><lib>

### 例子：服务端
  1. cd evl_net/test/test_server
  2. make
      注：Makefile 内容如下(还需手工导入third_party 所在路径) 

            test_server : _main_test_server.cc  
              g++ -I/usr/local/boost_1_61_0/ -L/usr/local/lib/evlnet -L/usr/local/lib/evllogger -L/usr/local/boost_1_61_0/stage/lib -llog4cplus -lpthread -levlnet -levl_logger -lboost_system -lboost_thread -D_PF_LINUX_ -o test_server _main_test_server.cc
    
  3. 运行 ./test_server 29999
  
            16-06-09 10:33:40> [evl_net:DEBUG] listening for new client to connect @ port:29999
  4. 有连接进来后，将显示：  

            16-06-09 10:35:26> [evl_net:INFO] new client connected 127.0.0.1:39223
            16-06-09 10:35:26> [evl_net:DEBUG] new client connected: 0x0xcc21c0
            16-06-09 10:35:26> [evl_net:DEBUG] [EVL_NET]client session:0xcc21c0 working...
            16-06-09 10:35:26> [evl_net:DEBUG] listening for new client to connect @ port:29999
      


### 例子：客户端
  1. cd evl_net/test/test_client
  2. make
      注：Makefile 内容如下  

            test_client : _main_test_client.cc  
              g++ -I/usr/local/boost_1_61_0/ -L/usr/local/lib/evlnet -L/usr/local/lib/evllogger -L/usr/local/boost_1_61_0/stage/lib -llog4cplus -lpthread -levlnet -levl_logger -lboost_system -lboost_thread -D_PF_LINUX_ -o test_client _main_test_client.cc
    
  3. 运行 ./test_client 29999
  4. 如果本地29999端口开启，将会收到连接成功的输出，如：  
  
            connecting to 29999 ...  
            16-06-09 09:49:51> [evl_net:DEBUG] [EVL_NET] connected to target endpoint succeed.  
            16-06-09 09:49:51> [evl_net:DEBUG] [EVL_NET]client session:0x7f2b780049d0 working...  
            connected to remote server`  
      
