
typedef string chain<256>;

typedef chain mchains<>;

program fildistributor{
    version distrver{
        int registerUser(string user) = 1;
        int unregisterUser(string user) = 2;
        int connectUser(string user, string ip, int port)=3;
        int disconnectUser(string user)=4;
        int publishFile(string user, string file, string description)=5;
        int deleteFile(string user, string file)=6;
        mchains listUser(string user)=7;
        mchains listContent(string user, string puser)=8;
    }=1;
}=99;
