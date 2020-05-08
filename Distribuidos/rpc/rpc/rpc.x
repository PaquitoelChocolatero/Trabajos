struct chain{
    string res<1024>;
};
program fildistributor{
    version distrver{
        void startServer()=1;
        void stopServer()=2; 
        int registerUser(string user) = 3;
        int unregisterUser(string user) = 4;
        int connectUser(string user, string ip, int port)=5;
        int disconnectUser(string user)=6;
        int publishFile(string user, string file, string description)=7;
        int deleteFile(string user, string file)=8;
        chain listOneUser(int usernumber)=9;
        chain listOneContent(int one)=10;
        int comprobar(string user)=11;
    }=1;
}=99;
