#include <iostream>
#include<mysql.h>
#include<mysqld_error.h>
#include<windows.h>

using namespace std;
const char *HOST="localhost";
const char *USER="root";
const char *PW="harshit113355";
const char * DB="authorization";
 
 class Login{
 	private:
 		string userID,userPW;
    public:
    	Login(): userID(""),userPW(""){}
    	
 void setID(string id){
 	userID=id;
 }
 void setPW(string pw){
 	userPW=pw;
 }
  string getID() const{
  	
  	return userID;
  }
   string getPW() const{
   	
  	return userPW;
  	
  }
  
 };

char encrypt(char ch,int shift){
	if(isalpha(ch)){
		
		char base=isupper(ch)?'A':'a'; 
		
		char encrypt_pass=(ch-base+shift+26)%26+base;
		
	return encrypt_pass;
	}
	if(isdigit(ch)){
		
		char encrypt_pass=(ch-'0'+shift+10)%10+'0';
		
		return encrypt_pass;
	}
	return ch;
}
 string encrypts(const string &password,int shift){
 	
 	string encrypted="";
 	
 	for(int i=0;i<password.length();i++){
 		
 		char ch=password[i];
 		
 		char encryptedChar=encrypt(ch,shift);
 		
 		encrypted+=encryptedChar;
	 }
	 return encrypted;
 }
 char decrypt(char ch,int shift){
	if(isalpha(ch)){
		
		char base=isupper(ch)?'A':'a'; 
		
		char decrypt_pass=(ch-base-shift+26)%26+base;
		
	return decrypt_pass;
	}
	if(isdigit(ch)){
		char decrypt_pass=(ch-'0'+shift+10)%10+'0';
		return decrypt_pass;
	}
	return ch;
}
 string decrypts(const string &password,int shift){
 	string decrypted="";
 	for(int i=0;i<password.length();i++){
 		char ch=password[i];
 		char decryptedChar=decrypt(ch,shift);
 		
 		decrypted+=decryptedChar;
	 }
	 return decrypted;
 }
 string matchPass(MYSQL *conn ,const string &id){
 	string encryptedPW;
 	string get="SELECT PW FROM credential WHERE ID='"+id+"'";
 	if(mysql_query(conn,get.c_str())){
    		cout<<"Error:"<<mysql_error(conn)<<endl;
		}
		else{
			MYSQL_RES *res;
			res=mysql_store_result(conn);
			if(res){
				MYSQL_ROW row=mysql_fetch_row(res) ;
				if(row){
					encryptedPW=row[0];
				}
			}
		}
		return encryptedPW;
 }

int main(){
	
	Login l;
	
MYSQL *conn;
conn=mysql_init(NULL);

if(!mysql_real_connect(conn,HOST,USER,PW,DB,3306,NULL,0)){
	cout<<"ERROR:"<<mysql_error(conn)<<endl;
		
}
else{
	cout<<"CONNECTING TO DATABASE!"<<endl;
}
Sleep(3000);

int shift=3;

bool exit=false;
while(!exit){
	cout<<"1.SIGNUP"<<endl;
	cout<<"2.LOGIN"<<endl;
	cout<<"3.EXIT"<<endl;
	cout<<"Enter Your Choice";
int val;
cin>>val;
    switch(val){
	
    case 1:{
		
	
    	system("cls");
    	string id,pw;
    	cout<<"Enter ID for signup: ";
    	cin>>id;
    	l.setID(id);
    	cout<<"Enter Password: ";
    	cin>>pw;
    	l.setPW(pw);
    	string encrypted_pass=encrypts(l.getPW(),shift);
    	string Sup="INSERT INTO credential(ID,PW)VALUES('"+l.getID()+"','"+encrypted_pass+"')";
    	if(mysql_query(conn,Sup.c_str())){
    		cout<<"Error:"<<mysql_error(conn)<<endl;
		}
		else{
			cout<<"Signup Successfully"<<endl;
		}
		Sleep(3000);
    	break;
    }
    case 2:{
		
	
    	system("cls");
    	string id,pw;
    	cout<<"Enter ID:  ";
    	cin>>id;
    	cout<<"Enter Your Password:  ";
    	cin>>pw;
    	string  getdb=matchPass(conn,id);
    	if(!getdb.empty()){
    		string decryptedPW=decrypts(getdb,shift);
    		if(decryptedPW==pw){
    			cout<<"LOGGED IN!!!!"<<endl;
			}
    		else{
    			cout<<"Incorrect Password.Try Again"<<endl;
			}
		}
		else{
			cout<<"User Not found .Try again later"<<endl; 
		}
		Sleep(5000);
		
		break;
	}
	case 3:
		exit=true;
		cout<<"THANKYOU!"<<endl;
		break;
		
	default :
		cout<<"INVALID INPUT";
		break;
}
}
mysql_close(conn);
return 0;
}
	

