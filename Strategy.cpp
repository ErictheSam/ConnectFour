#include <random>
#include "time.h"
#include "Point.h"
#include "Judge.h"
#include "Strategy.h"
#include <stdio.h>
#include <iostream>
#include <cmath>

#define MAXI 12
#define VITALITY_COEFFICIENT 0.8
#define TIME_LIMIT 2998


INT first[12*12];

INT firstTop[12];

int times = 0;

class Status;
class UCT;

UCT * tree = NULL;

int row, col;

int nox, noy;


#define SIZE 4000000

using namespace std;


int point = 1;

int startTime;
Status * temp;
int temp2;

class Status{
	public:
		INT chessboard[MAXI*MAXI];
       //int mylongLink, opponentlongLink;
	   INT topNode[MAXI];
		INT expandableNode[MAXI];
		INT expandableNodeSize;
		INT _preX,_preY;//
		//char len;
		//char maxlen;
		int visitedNum; double profit;
		//int descendentSize;
        bool player;
        Status *father;//
        Status *descendents[MAXI];
		bool stop;

		bool Stops(){//
			if(isTie(col, topNode))//
				return true;
			if(player==true){
				if(machineWin(_preX,_preY,row,col,chessboard))
					return true;}
			else{
				if(userWin(_preX,_preY,row,col,chessboard))
					return true;
			}
			return false;
		}

	Status* BestChild(){//
		Status * bestcld;
		double maxProfitRatio = -RAND_MAX;
		for( int i = 0; i != col; i ++){
			temp = descendents[i];
			if( temp != NULL){
				int descendentCldNum = temp -> visitedNum;
				double modifiedProfit = (player == false?
					1:-1)*(temp -> profit);
				double tempProfitRatio = modifiedProfit/descendentCldNum + sqrtl(2 * logl(visitedNum) / descendentCldNum); //¼ÆËã×ÛºÏÊÕÒæÂÊ£¬¶à´Î°¡¶à´Î
				if (tempProfitRatio > maxProfitRatio || (tempProfitRatio == maxProfitRatio && rand() % 2 == 0)){ //Ñ¡Ôñ×ÛºÏÊÕÒæÂÊ×î´óµÄ×Ó½Ú
					maxProfitRatio = tempProfitRatio;
					bestcld = temp;
				}
			}
		}
		return bestcld;
	}

	void init ( char prex, char prey , Status *_father){

		expandableNodeSize = 0;
		profit = 0;
		player = !(_father -> player);
		father = _father;
		INT * head1 = father->chessboard;INT* head2 = father->topNode;
	    visitedNum = 0;
		_preX = prex;
		_preY = prey;
		for( int i = 0; i < row*col; i ++)
			chessboard[i] = head1[i];
		for( int i = 0; i < col; i ++){
			topNode[i] = head2[i];
			descendents[i] = NULL;
		}
		topNode[_preY]-=1;
		if(topNode[_preY]-1 == nox && _preY == noy){
			topNode[_preY] = nox;
		}
		for( int i = 0; i < col; i ++){
			if(topNode[i] > 0 )
				expandableNode[expandableNodeSize ++] = i;
		}
	}
	Status* Expand(){
		int index = rand()%expandableNodeSize;
		int i = expandableNode[index];

		Status *temp = status + point;

		temp->init(topNode[i]-1,i,this);
		temp->chessboard[(topNode[i]-1)*col+i] = player == true? 1:2;//player是true的话自己下了1
		descendents[i] = temp;
		expandableNodeSize --;
		swap(expandableNode[index],expandableNode[ expandableNodeSize ]);
		point ++;
		temp -> stop = temp->Stops();
		return temp;
	}

}status[SIZE];

int stt;

void backup( Status * temp,double semiProfit) {//
	//stt = 0;
	while(temp != NULL ) {

		//stt ++;
		temp -> visitedNum ++;
		temp -> profit += semiProfit; //ÊÕÒæÔö¼Ódelta
		temp = temp -> father;
		}
	}

bool player;
INT chessman;
int x,y;

inline bool End(){
	if(isTie(col, firstTop))
		return true;

	if(player==true){
		if(machineWin(x,y,row,col,first))
			return true;}
	else{
		if(userWin(x,y,row,col,first))
			return true;
	}
	return false;
}

int secondTop[12];

Status * temped = NULL;
class UCT
{
	public:
		Status * _root;


		UCT(Status * root):_root(root){}

		Status * TreePolicy(Status * present/*,bool &heimat*/){//ÊÇ·ñ»á
			while( !(present->stop)){
				if( present->expandableNodeSize > 0){//can nicht expand
					return (present->Expand());

				}
				else{
					present = present->BestChild();
				}
			}
			return present;
		}

		void DefaultPolicy(Status * head){

			INT * head1 = head->chessboard;INT* head2 = head->topNode;
			for(int i = 0; i < row*col; i ++)
				first[i] =head1[i];
			for(int i = 0; i <col; i ++)
				firstTop[i] = head2[i];
			player = head->player;
			x = head->_preX;
			y = head->_preY;
			int dept = 0;
			while(!End()){
				chessman = (player == false)?2:1;
				y = rand() % col;
				while (firstTop[y] <= 0){
					y = rand() % col;
				}
				x = -- firstTop[y];
				first[x*col+ y] = chessman; //落子
				if (x - 1 == nox && y == noy) //若落子位置正上方紧邻不可落子点
					firstTop[y] --;
				player = !player;
			}
			double deltaProfit;
			if(isTie(col,firstTop))
				deltaProfit = 0;
			else
				deltaProfit = player == true?1:-1;
			backup(head,deltaProfit);
		}

	Point* UCTSearch( ){
			temped = _root;
			while(clock()-startTime <= 2982 ) {
				Status* selectedStatus = TreePolicy(_root/*,fst*/);
				DefaultPolicy(selectedStatus);
			}
			printf("%d ",clock()-startTime);
			Status * temp = _root->BestChild();
			Point * i = new Point(temp->_preX,temp->_preY) ;
			printf("%d %d\n",point,clock()-startTime);
//printf("%d\n",point);
			return i;
		}

};



/*
	²ßÂÔº¯Êý½Ó¿Ú,¸Ãº¯Êý±»¶Ô¿¹Æ½Ì¨µ÷ÓÃ,Ã¿´Î´«Èëµ±Ç°×´Ì¬,ÒªÇóÊä³öÄãµÄÂä×Óµã,¸ÃÂä×Óµã±ØÐëÊÇÒ»¸ö·ûºÏÓÎÏ·¹æÔòµÄÂä×Óµã,²»È»¶Ô¿¹Æ½Ì¨»áÖ±½ÓÈÏÎªÄãµÄ³ÌÐòÓÐÎó

	input:
		ÎªÁË·ÀÖ¹¶Ô¶Ô¿¹Æ½Ì¨Î¬»¤µÄÊý¾ÝÔì³É¸ü¸Ä£¬ËùÓÐ´«ÈëµÄ²ÎÊý¾ùÎªconstÊôÐÔ
		M, N : ÆåÅÌ´óÐ¡ M - ÐÐÊý N - ÁÐÊý ¾ù´Ó0¿ªÊ¼¼Æ£¬ ×óÉÏ½ÇÎª×ø±êÔ­µã£¬ÐÐÓÃx±ê¼Ç£¬ÁÐÓÃy±ê¼Ç
		top : µ±Ç°ÆåÅÌÃ¿Ò»ÁÐÁÐ¶¥µÄÊµ¼ÊÎ»ÖÃ. e.g. µÚiÁÐÎª¿Õ,Ôò_top[i] == M, µÚiÁÐÒÑÂú,Ôò_top[i] == 0
		_board : ÆåÅÌµÄÒ»Î¬Êý×é±íÊ¾, ÎªÁË·½±ãÊ¹ÓÃ£¬ÔÚ¸Ãº¯Êý¸Õ¿ªÊ¼´¦£¬ÎÒÃÇÒÑ¾­½«Æä×ª»¯ÎªÁË¶þÎ¬Êý×éboard
				ÄãÖ»ÐèÖ±½ÓÊ¹ÓÃboard¼´¿É£¬×óÉÏ½ÇÎª×ø±êÔ­µã£¬Êý×é´Ó[0][0]¿ªÊ¼¼Æ(²»ÊÇ[1][1])
				board[x][y]±íÊ¾µÚxÐÐ¡¢µÚyÁÐµÄµã(´Ó0¿ªÊ¼¼Æ)
				board[x][y] == 0/1/2 ·Ö±ð¶ÔÓ¦(x,y)´¦ ÎÞÂä×Ó/ÓÐÓÃ»§µÄ×Ó/ÓÐ³ÌÐòµÄ×Ó,²»¿ÉÂä×Óµã´¦µÄÖµÒ²Îª0
		lastX, lastY : ¶Ô·½ÉÏÒ»´ÎÂä×ÓµÄÎ»ÖÃ, Äã¿ÉÄÜ²»ÐèÒª¸Ã²ÎÊý£¬Ò²¿ÉÄÜÐèÒªµÄ²»½ö½öÊÇ¶Ô·½Ò»²½µÄ
				Âä×ÓÎ»ÖÃ£¬ÕâÊ±Äã¿ÉÒÔÔÚ×Ô¼ºµÄ³ÌÐòÖÐ¼ÇÂ¼¶Ô·½Á¬Ðø¶à²½µÄÂä×ÓÎ»ÖÃ£¬ÕâÍêÈ«È¡¾öÓÚÄã×Ô¼ºµÄ²ßÂÔ
		noX, noY : ÆåÅÌÉÏµÄ²»¿ÉÂä×Óµã(×¢:ÆäÊµÕâÀï¸ø³öµÄtopÒÑ¾­ÌæÄã´¦ÀíÁË²»¿ÉÂä×Óµã£¬Ò²¾ÍÊÇËµÈç¹ûÄ³Ò»²½
				ËùÂäµÄ×ÓµÄÉÏÃæÇ¡ÊÇ²»¿ÉÂä×Óµã£¬ÄÇÃ´UI¹¤³ÌÖÐµÄ´úÂë¾ÍÒÑ¾­½«¸ÃÁÐµÄtopÖµÓÖ½øÐÐÁËÒ»´Î¼õÒ»²Ù×÷£¬
				ËùÒÔÔÚÄãµÄ´úÂëÖÐÒ²¿ÉÒÔ¸ù±¾²»Ê¹ÓÃnoXºÍnoYÕâÁ½¸ö²ÎÊý£¬ÍêÈ«ÈÏÎªtopÊý×é¾ÍÊÇµ±Ç°Ã¿ÁÐµÄ¶¥²¿¼´¿É,
				µ±È»Èç¹ûÄãÏëÊ¹ÓÃlastX,lastY²ÎÊý£¬ÓÐ¿ÉÄÜ¾ÍÒªÍ¬Ê±¿¼ÂÇnoXºÍnoYÁË)
		ÒÔÉÏ²ÎÊýÊµ¼ÊÉÏ°üº¬ÁËµ±Ç°×´Ì¬(M N _top _board)ÒÔ¼°ÀúÊ·ÐÅÏ¢(lastX lastY),ÄãÒª×öµÄ¾ÍÊÇÔÚÕâÐ©ÐÅÏ¢ÏÂ¸ø³ö¾¡¿ÉÄÜÃ÷ÖÇµÄÂä×Óµã
	output:
		ÄãµÄÂä×ÓµãPoint
*/

Status * now;

//int oppolink = 0, thislink = 0;

extern "C" __declspec(dllexport) Point* getPoint(const int M, const int N, const int* top, const int* _board,
	const int lastX, const int lastY, const int noX, const int noY){
	startTime = clock();
//	cout << "in" << endl;
	point = 1;
	//cout << point << endl;
	int x = -1; int y = -1;
	//cout << "in" << endl;
	//Status * out;
	row = M; col = N;
	nox = noX; noy = noY;
//	int ans = 0;
	int expandsz = 0 ;
	status->profit = 0;
	status->player = false;
	status->father = NULL;
	status->visitedNum = 0;
	for( int p = 0; p < M*N; p ++)
		status->chessboard[p] = _board[p];
	for( int q = 0; q < N; q ++){
		status->topNode[q] = top[q];
		if(top[q] != 0)
			status->expandableNode[expandsz ++] = q;
		status->descendents[q] = NULL;
	}
	status->father = NULL;
	status->expandableNodeSize = expandsz;
	if(lastX > -1 && lastY > -1)
		status->chessboard[lastX*N+lastY]=1;
	status -> stop = status -> Stops();
	status->_preX = lastX;
	status->_preY = lastY;
	tree = new UCT(status);
	return (tree -> UCTSearch());

}


/*
	getPointº¯Êý·µ»ØµÄPointÖ¸ÕëÊÇÔÚ±¾dllÄ£¿éÖÐÉùÃ÷µÄ£¬Îª±ÜÃâ²úÉú¶Ñ´íÎó£¬Ó¦ÔÚÍâ²¿µ÷ÓÃ±¾dllÖÐµÄ
	º¯ÊýÀ´ÊÍ·Å¿Õ¼ä£¬¶ø²»Ó¦¸ÃÔÚÍâ²¿Ö±½Ódelete
*/
extern "C" __declspec(dllexport) void clearPoint(Point* p){
	delete p;
	return;
}

/*
	Çå³ýtopºÍboardÊý×é
*/
void clearArray(int M, int N, int** board){
	for(int i = 0; i < M; i++){
		delete[] board[i];
	}
	delete[] board;
}



/*
	Ìí¼ÓÄã×Ô¼ºµÄ¸¨Öúº¯Êý£¬Äã¿ÉÒÔÉùÃ÷×Ô¼ºµÄÀà¡¢º¯Êý£¬Ìí¼ÓÐÂµÄ.h .cppÎÄ¼þÀ´¸¨ÖúÊµÏÖÄãµÄÏë·¨
*/
