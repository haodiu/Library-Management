#include <iostream>
#include <mylib.h>
#include <ctime>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib> 
#include <ctime>
using namespace std;
const int MAX_DAUSACH = 300; 
const int MAX_DOCGIA = 200;
const int QUEUESIZE = 200;
const int SO_ITEM = 6;
const int COT_A = 38;
const int DONG_A = 18;
const int UP = 72;
const int DOWN = 80;
const int LEFT = 75;
const int RIGHT = 77;
const int ENTER = 13;
const int BACK_SPACE = 8;
const int ESC = 27;
const int W = 123;
const int H = 42;
const int BIENTRAI = 10;
const int MAX_MADOCGIA = 1000;
const int MAX_MASACH = 1000;
//-------SACH---------
struct SACH{
	string masach;
	int trangthai; //0: cho muon duoc;  1: da co doc gia muon;  2: da thanh ly;
	string vitri;
};

struct NODE_SACH{
	SACH sach;
	NODE_SACH *next;
};
void InsertFirst_NodeSach(NODE_SACH* &First, SACH &s){
	NODE_SACH *p = new NODE_SACH;
	p->sach = s;
	p->next = First;
	First = p;
}
void ClearDMS(NODE_SACH* &First){
	NODE_SACH* p;
	while(First != NULL){
		p = First;
		First = First->next;
		delete p;
	}
}
struct DAUSACH{
	string ISBN;
	string tensach;
	string sotrang;
	string tacgia;
	string NXB;
	string theloai;
	//them 
	int soluong;
	int soluotmuon;
	
	NODE_SACH *dms = NULL;
};
struct DS_DAUSACH{
	DAUSACH *nodes[MAX_DAUSACH];
	int n=0;
};
bool Empty(DS_DAUSACH dsds){
	return dsds.n == 0;
}
bool Full(DS_DAUSACH dsds){
	return dsds.n == MAX_DAUSACH;
}
int AddDauSach(DS_DAUSACH &dsds, DAUSACH *dausach){
	if(Full(dsds)){
		cout<<"danh sach day";
		return 1;
	}
	dsds.nodes[dsds.n] = dausach;
	dsds.n++;
	return 0;
	}
bool TonTaiISBN(DS_DAUSACH dsds, string isbn){
	if(Full(dsds)){
		cout<<"danh sach day";
		return false;
	}
	for(int i=0; i<dsds.n; i++){
		if(dsds.nodes[i]->ISBN == isbn)
			return true;
	}
	return false;
}
void TimDauSachBangISBN(DS_DAUSACH dsds, string isbn, DAUSACH* &dausach){
	if(Empty(dsds)){
		cout<<"danh sach rong";
		return;
	}
	for(int i=0; i<dsds.n; i++){
		if(dsds.nodes[i]->ISBN == isbn){
			dausach = dsds.nodes[i];
			return;
		}
	}
}
void TimDauSachBangTen(DS_DAUSACH dsds, string tensach, DAUSACH* &dausach){
	if(dsds.n == 0){
		cout<<"danh sach rong";
		return;
	}
	for(int i=0; i<dsds.n; i++){
		if(dsds.nodes[i]->tensach == tensach){
			*dausach = *dsds.nodes[i];
			return;
		}
	}
}
int GetPositionNodeSach(DS_DAUSACH dsds, string isbn){
	if(Empty(dsds))  return -1;
	for(int i = 0; i < dsds.n; i++){
		if(isbn == dsds.nodes[i]->ISBN)  return i;
	}
	return -1;
}

int Delete_DauSach(DS_DAUSACH &dsds, string isbn){
	int vitri = GetPositionNodeSach(dsds, isbn);
	if(vitri == -1 || vitri > dsds.n || Empty(dsds))  return 1;
	if(vitri == dsds.n-1){
		delete dsds.nodes[dsds.n-1];
		dsds.nodes[dsds.n-1] = NULL;
		dsds.n--;
		return 0;
	}
	delete dsds.nodes[vitri];
	for(int i = vitri; i < dsds.n; i++)
		dsds.nodes[i] = dsds.nodes[i+1];
	delete dsds.nodes[dsds.n-1];
	dsds.nodes[dsds.n-1] = NULL;
	dsds.n--;
	return 0;
}
string CoverTrangThaiSach(int trangthai){
	string chomuonduoc = "CHO MUON DUOC";
	string dacodocgiamuon = "DA CO DOC GIA MUON";
	string dathanhly = "DA THANH LY";
	if(trangthai == 0) return chomuonduoc;
	else if(trangthai == 1)  return dacodocgiamuon;
	return dathanhly;
}
bool DauSachDuocMuon(DS_DAUSACH dsds, string isbn){
	if(Empty(dsds))  return false;
	for(int i=0; i<dsds.n; i++){
		if(isbn == dsds.nodes[i]->ISBN){
			for(NODE_SACH *p = dsds.nodes[i]->dms; p!= NULL; p = p->next){
				if(p->sach.trangthai == 1)
					return true;
			}
		}
	}
	return false;
}
bool TonTaiSach(DAUSACH* dsach, SACH s){
	if(dsach->dms == NULL){
		cout<<"dms rong";
		return false;
	}
	for(NODE_SACH *p = dsach->dms; p!=NULL; p = p->next){
		if(p->sach.masach == s.masach)
			return true;
	}
	return false;
}
void DeleteMemoryDMS(DS_DAUSACH &dsds){
	for(int i=0; i<dsds.n; i++){
		ClearDMS(dsds.nodes[i]->dms);
		delete dsds.nodes[i];
	}
}
//-------MUONTRA-------------
struct MUONTRA{
	string MASACH;
	char NgayMuon[11];
	char NgayTra[11];
	int trangthai;  // 0: sach dang muon(chua tra),  1: da tra,  2: lam mat sach;
};
struct NODE_MUONTRA{
	MUONTRA data;
	NODE_MUONTRA *next;
};
struct DS_MUONTRA{
	NODE_MUONTRA *First = NULL;
	//them
	int total = 0; //tong sach da muon + dang muon
	int chuatra = 0;
};
void InsertFirst_MuonTra(DS_MUONTRA &DSMT, MUONTRA &mt){
	NODE_MUONTRA *p = new NODE_MUONTRA;
	p->data = mt;
	p->next = DSMT.First;
	DSMT.First = p;
	DSMT.total++;
	if(mt.trangthai != 1)  DSMT.chuatra++;////////////////////
}
void Traverse_MuonTra(DS_MUONTRA DSMT, MUONTRA* dsmuontra, int trangthai){
	if(DSMT.First == NULL){
		cout<<"DSMT rong";
		return;
	}
	int i=0;
	NODE_MUONTRA* p;
	for(p = DSMT.First; p != NULL; p = p->next){
		if(p->data.trangthai == trangthai)
			dsmuontra[i++] = p->data;
	}
}
void ClearDSMT(DS_MUONTRA &DSMT){
	NODE_MUONTRA *p;
	while(DSMT.First != NULL){
		p = DSMT.First;
		DSMT.First = p->next;
		delete p;
	}
}
//--------DANH SACH THE DOC GIA-----------
struct DOCGIA{
	int MATHE;
	string ho;
	string ten;
	string phai; 
	int trangthai; //0 khoa, 1 hoat dong
	DS_MUONTRA dsmuontra ;
};
struct NODE_DOCGIA{
	DOCGIA docgia;
	NODE_DOCGIA *left = NULL;
	NODE_DOCGIA *right = NULL;
};
void Initialize(NODE_DOCGIA* &root){
   root = NULL;
}
int i=0;
void InOrder(NODE_DOCGIA* &p, DOCGIA* dg, int soluong){
	if(p != NULL){  
		InOrder(p->left, dg, soluong);
		dg[i++] = p->docgia;
		if(i==soluong)  i = 0;
		InOrder(p->right, dg, soluong);
	}
}
void Insert_DocGia(NODE_DOCGIA* &p, DOCGIA &dg){
	if(p == NULL){
		p = new NODE_DOCGIA;
		p->docgia = dg;
		p->left = NULL;
		p->right = NULL;
	}else{
		if(dg.MATHE < p->docgia.MATHE)
			Insert_DocGia(p->left, dg);
		else if(dg.MATHE > p->docgia.MATHE)
			Insert_DocGia(p->right, dg);
	}
}
bool Empty_Tree(NODE_DOCGIA* p){
	return (p == NULL ? true : false);
}
void RemoveDocGia_Case3(NODE_DOCGIA* &p, NODE_DOCGIA* &rp){
	if(p->left != NULL)
		RemoveDocGia_Case3(p->left, rp);
	else{
		rp->docgia = p->docgia;
		rp = p;
		p = p->right;
	}
}
int RemoveDocGia(NODE_DOCGIA* &p, int madocgia){
	if(Empty_Tree(p))	return 0;
	if(madocgia < p->docgia.MATHE)	RemoveDocGia(p->left, madocgia);
	else if(madocgia > p->docgia.MATHE)  RemoveDocGia(p->right, madocgia);
	else{
		NODE_DOCGIA *rp = p;
		if(p->right == NULL)	p = rp->left;
		else if(p->left == NULL)	p = rp->right;
		else
			RemoveDocGia_Case3(p->right, rp);
		delete rp;
	}
	return 1;
}
NODE_DOCGIA* SearchDG(NODE_DOCGIA* &TreeDG, int madocgia){
	NODE_DOCGIA* p = TreeDG;
	while(p != NULL && p->docgia.MATHE != madocgia){
		if(madocgia < p->docgia.MATHE)
			p = p->left;
		else
			p = p->right;
	}
	return p;
}
void DeleteMemoryDocGia(NODE_DOCGIA* &node){
	if(node != NULL){
		DeleteMemoryDocGia(node->left);
		DeleteMemoryDocGia(node->right);
		ClearDSMT(node->docgia.dsmuontra);
		delete node;
	}
}
template <typename Type>
struct Queue{
	int front, rear;
	Type *nodes;	

	Queue(){
		front = -1; rear = -1;
		nodes = new Type[QUEUESIZE];
	}
	~Queue(){
		delete[] nodes;
	}	
	bool empty(){
		return((front==-1 || rear==-1) ? true : false);
	}
	Type GetFront(){
		return nodes[front];
	}	
	int Insert_queue(Type value){
		if(rear-front+1 == 0 || rear-front+1 == QUEUESIZE)	return 0;
		if(front == -1){
			front = 0;
			rear = -1;
		}
		if(rear == QUEUESIZE-1)  rear = -1;
		++rear;
		nodes[rear] = value;
		return 1;
	}
	int Delete_queue(){
		if(empty())  return 0;
		if(front == rear){
			front = -1;
			rear = -1;
		}else{
			front++;
			if(front == QUEUESIZE)  front = 0;
		}
		return 1;
	}
};
struct LUOTMUONSACH{
	int indexDS;
	int luotmuon;
};
struct TOPSACH{
	int n;
	LUOTMUONSACH *list;
	
	TOPSACH(DS_DAUSACH &DSDS, bool &IsNull){
		list = new LUOTMUONSACH[DSDS.n];
		int dem = 0;
		for(int i=0; i<DSDS.n; i++){
			if(DSDS.nodes[i]->soluotmuon > 0){
				list[dem].indexDS = i;
				list[dem].luotmuon = DSDS.nodes[i]->soluotmuon;
				dem++;
			}
		}
		if(dem == 0) IsNull = true;
		else{
			n = dem;
			IsNull = false;
			InsertionSort();
		}
	}	
	void InsertionSort(){
		int i, j;
		LUOTMUONSACH x;
		for(i=1; i<n; i++){
			x = list[i];
			for(j=i-1; j>=0 && x.luotmuon>list[j].luotmuon; j--)
				list[j+1] = list[j];
			list[j+1] = x;
		}
	}
};
////////////////////////
NODE_DOCGIA *root = NULL;	
DS_DAUSACH DSDS;
//===========Windows.h===============================
void resizeConsole(int width, int height)
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);
}
void GetWindowSize(int &Width, int &Height) 
{ 
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    Width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    Height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}
void SetScreenBufferSize(SHORT width, SHORT height)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD NewSize;
    NewSize.X = width;
    NewSize.Y = height;

    SetConsoleScreenBufferSize(hStdout, NewSize);
}
void Setcolor(WORD color)
{
    HANDLE hConsoleOutput;
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
    GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);

    WORD wAttributes = screen_buffer_info.wAttributes;
    color &= 0x000f;
    wAttributes &= 0xfff0;
    wAttributes |= color;

    SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}
void ShowCur(bool CursorVisibility)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ConCurInf;
    
    ConCurInf.dwSize = 10;
    ConCurInf.bVisible = CursorVisibility;
    
    SetConsoleCursorInfo(handle, &ConCurInf);
}

//===========ITEM MENU DONG===============
string ThucDon[SO_ITEM] = {"XU LY DOC GIA", "DANH SACH DOC GIA", "XU LY DAU SACH", "DANH SACH CAC DAU SACH", "MUON & TRA SACH", "THOAT"};		            
string SuaDauSach[8] = {"LUU", "ISBN", "THE LOAI", "TEN SACH", "TAC GIA", "SO TRANG", "NXB", "SO LUONG"};
string HienThiDocGia[3] = {"HO & TEN DOC GIA TANG DAN", "MA DOC GIA TANG DAN", "DOC GIA MUON SACH QUA HAN"};
string NhapDocGia[3] = {"THEM DOC GIA", "XOA DOC GIA", "HIEU CHINH DOC GIA"};
string XuLyDauSach[3] = {"THEM DAU SACH", "XOA DAU SACH", "HIEU CHINH DAU SACH"};
string DSDauSach [2] = {"TAT CA DAU SACH", "TOP 10 DAU SACH DUOC MUON NHIEU NHAT"};
string MuonSach[2] = {"MUON SACH", "TRA SACH"};
string Menu_HieuChinh[5] = {"LUU", "HO", "TEN", "GIOI TINH", "TRANG THAI"};				
//=============GIAODIEN=======================================================================================
void TieuDe (int x, int y)
{
	SetColor(7);
	gotoxy(x+12, y + 1); cout << " ________    _________    _    _________ ";
	gotoxy(x+12, y + 2); cout << "|  ____  |  |___   ___|  | |  |___   ___|";
	gotoxy(x+12, y + 3); cout << "| |____| |      | |      | |      | |    ";
	gotoxy(x+12, y + 4); cout << "|  ______/      | |      | |      | |    ";
	gotoxy(x+12, y + 5); cout << "| |             | |      | |      | |    ";
	gotoxy(x+12, y + 6); cout << "| |             | |      | |      | |    ";
	gotoxy(x+12, y + 7); cout << "|_|             |_|      |_|      |_|    ";
	
	SetColor(9);
	gotoxy(x, y + 11);  cout <<" _        _   _______   _______        _       _______  _     _           ";
	gotoxy(x, y + 12); cout << "| |      | | |  ___  | |  ___  |      / \\     |  ___  | \\ \\  / /       ";
	gotoxy(x, y + 13); cout << "| |      | | | |___/ / | |___/ /     / _ \\    | |___/ /  \\ \\/ /        ";
	gotoxy(x, y + 14); cout << "| |      | | |  ___ |  |  ___ |     / /_\\ \\   |  ___ |    \\  /         ";
	gotoxy(x, y + 15); cout << "| |____  | | | |___\  \\ | |   \\ \\   / /   \\ \\  | |  \\ \\    / /     ";
	gotoxy(x, y + 16); cout << "|______| |_| |_______| |_|    \\_\\ /_/     \\_\\ |_|   \\_\\  /_/        ";     
	SetColor(11);
	gotoxy(x+14, y+23); cout<<"     DE TAI: QUAN LY THU VIEN";
	gotoxy(x+14, y+25); cout<<"  GV HUONG DAN: LUU NGUYEN KY THU";
	gotoxy(x+14, y+27); cout<<"        LOP: D19CQCN02-N";
	gotoxy(x+14, y+29); cout<<"NHOM 3: N19DCCN049 - NGUYEN ANH HAO";
	SetColor(12);
	gotoxy(x+14, y+35); cout<<"          PRESS ANY KEY";
	SetColor(15);
}
void OutChar(int x, int y, char data){
	gotoxy(x,y);
	cout<<data;
}
struct ToaDo{
	int x, y;
	ToaDo(int a, int b){
		x = a;
		y = b;
	}
};
void Line(ToaDo start, int length, short direct, int character){
	int i;
	//direct = 0: ngang, 1: doc
	if(direct == 0){
		for(i = start.x; i <= length; i++)
			OutChar(i, start.y, (char)character);
	}
	else if(direct == 1){
		for(i = start.y; i<=length; i++)
			OutChar(start.x, i, (char)character);
	}
}
void Box(ToaDo A, ToaDo C){
	OutChar(A.x, A.y,(char)218);
	OutChar(C.x, A.y,(char)191);
	OutChar(C.x, C.y,(char)217);
	OutChar(A.x, C.y,(char)192);
	Line(ToaDo(A.x+1, A.y), C.x-1,0,(char)196);
	Line(ToaDo(A.x+1, C.y), C.x-1,0,(char)196);
	Line(ToaDo(A.x, A.y+1), C.y-1,1,(char)179);
	Line(ToaDo(C.x, A.y+1),C.y-1,1,(char)179);
}
void ColorBox(ToaDo A, ToaDo C, string text,  int BGcolor, int textcolor){
	int rong = C.x-A.x;
	int cao = (A.y+C.y)/2;
	int vitriText =A.x + (rong-text.length())/2;
	SetBGColor(BGcolor);
	for(int j = 0; j<C.y-A.y+1; j++){
		gotoxy(A.x, A.y+j);
		for(int i = A.x+1; i<=C.x+1; i++){
			cout<<" ";
		}
		gotoxy(vitriText, cao);
		SetColor(textcolor);
		cout<<text;
		ShowCur(false);
	}
}	                  
void Khung(){
	int i;
	OutChar(1,0,(char)201);
	OutChar(1,H-1,(char)200);
	OutChar(W-1,0,(char)187);
	OutChar(W-1,H-1,(char)188);
	for(i=2; i<W-1;i++){
		OutChar(i,0,(char)205);
		OutChar(i,H-1,(char)205);
	}
	for(i=1; i<H-1;i++){
		OutChar(1,i,(char)186);
		OutChar(W-1,i,(char)186);
	}
}
void XoaNoiDung(int y, int sodong, int color){  
	SetBGColor(color);
	for(int i=0; i<sodong; i++){
		gotoxy(2,y+i);  
		cout<<"                                                                                                                        "<<endl;
	}
}
int MenuDong(string td[], int x, int y){
	SetColor(11);
	gotoxy(x+9, y+29);   cout<<"---------------------------------";
	gotoxy(x-1, y+30);  cout<<"LUU Y: bam <ESC> hoac go <ESC> vao o trong de tro ve";
	gotoxy(x+14, y+31);   cout<<"______________________";
	int cot = x, dong = y;
	int chon = 0;
	for(int i=0; i<SO_ITEM; i++){
		ColorBox(ToaDo(cot, dong+4*i), ToaDo(cot+50, dong+2+4*i), td[i], 14, 0);
	}
	ColorBox(ToaDo(cot, dong+4*chon), ToaDo(cot+50, dong+2+4*chon), td[chon], 1, 15);
	char kytu;
	do{
		kytu = getch(); 
		switch(kytu){
			case UP: if(chon+1 > 1){
				ColorBox(ToaDo(cot, dong+4*chon), ToaDo(cot+50, dong+2+4*chon), td[chon], 14, 0);
				chon--;
				ColorBox(ToaDo(cot, dong+4*chon), ToaDo(cot+50, dong+2+4*chon), td[chon], 1, 15);
				SetBGColor(0);
			}break;
			case DOWN: if(chon+1 < SO_ITEM){
				ColorBox(ToaDo(cot, dong+4*chon), ToaDo(cot+50, dong+2+4*chon), td[chon], 14, 0);
				chon++;
				ColorBox(ToaDo(cot, dong+4*chon), ToaDo(cot+50, dong+2+4*chon), td[chon], 1, 15);
				SetBGColor(0);
			}break;
			case 13: {
				XoaNoiDung(1, 39, 0);
				return chon+1;
			}
		}
	}while(true);
} 
int MenuCon(string menu[], int so_item){
	int x = 7;
	int y = 2;
	int rong = (W-20)/so_item;
	int kc_2box = 4;
	for(int i=0; i<so_item; i++){
		ColorBox(ToaDo(x+(rong+kc_2box)*i,y), ToaDo(x+(rong+kc_2box)*i+rong,y+2), menu[i], 14, 0);
	}
	int chon = 0;
	ColorBox(ToaDo(x+(rong+kc_2box)*chon,y), ToaDo(x+(rong+kc_2box)*chon+rong,y+2), menu[chon], 1, 15);
	char kytu;
	while(true){
		kytu = getch();
		switch(kytu){
			case LEFT:	if(chon+1 > 1){
						ColorBox(ToaDo(x+(rong+kc_2box)*chon,y), ToaDo(x+(rong+kc_2box)*chon+rong,y+2), menu[chon], 14, 0);
						chon--;
						ColorBox(ToaDo(x+(rong+kc_2box)*chon,y), ToaDo(x+(rong+kc_2box)*chon+rong,y+2), menu[chon], 1, 15);
						SetBGColor(0);
						}break;
			case RIGHT: if(chon+1 < so_item){
						ColorBox(ToaDo(x+(rong+kc_2box)*chon,y), ToaDo(x+(rong+kc_2box)*chon+rong,y+2), menu[chon], 14, 0);
						chon++;
						ColorBox(ToaDo(x+(rong+kc_2box)*chon,y), ToaDo(x+(rong+kc_2box)*chon+rong,y+2), menu[chon], 1, 15);
						SetBGColor(0);
						}break;
			case ENTER: {
				SetBGColor(0);
				return chon+1;
			}
			case ESC: {
				XoaNoiDung(1, 39, 0);
				return -1;
			}
		}
	}
}
int MenuNgang(string str[], int x, int y, int so_item){
	int rong = 15;
	int kc_2box = 2;
	int chon = 0;
	for(int i = 0; i < so_item; i++){
		ColorBox(ToaDo(x+(rong+kc_2box)*i,y), ToaDo(x+(rong+kc_2box)*i+rong,y+2), str[i], 15, 0);
	}
	ColorBox(ToaDo(x+(rong+kc_2box)*chon,y), ToaDo(x+(rong+kc_2box)*chon+rong,y+2), str[chon], 1, 15);
	char kytu;
	while(true){
		kytu = getch();
		switch(kytu){
			case LEFT:	if(chon+1 > 1){
						ColorBox(ToaDo(x+(rong+kc_2box)*chon,y), ToaDo(x+(rong+kc_2box)*chon+rong,y+2), str[chon], 15, 0);
						chon--;
						ColorBox(ToaDo(x+(rong+kc_2box)*chon,y), ToaDo(x+(rong+kc_2box)*chon+rong,y+2), str[chon], 1, 15);
						SetBGColor(0);
						}break;
			case RIGHT: if(chon+1 < so_item){
						ColorBox(ToaDo(x+(rong+kc_2box)*chon,y), ToaDo(x+(rong+kc_2box)*chon+rong,y+2), str[chon], 15, 0);
						chon++;
						ColorBox(ToaDo(x+(rong+kc_2box)*chon,y), ToaDo(x+(rong+kc_2box)*chon+rong,y+2), str[chon], 1, 15);
						SetBGColor(0);
						}break;
			case ENTER: {
				SetBGColor(0);
				return chon+1;
			}
			case ESC: return -1;
		}
	}
}
int MenuDoc(string str[], int x, int y, int so_item, int rong, int cao){
	int cot = x, dong  = y;
	for(int i=0; i<so_item; i++)
		ColorBox(ToaDo(cot, dong+i), ToaDo(cot+rong, dong+cao*i), str[i], 14, 0);
	int chon = 0;
	ColorBox(ToaDo(cot, dong+chon), ToaDo(cot+rong, dong+cao*chon), str[chon], 1, 15);
	char kytu;
	while(true){
		kytu = getch(); 
		switch(kytu){
			case UP: if(chon+1 > 1){
				ColorBox(ToaDo(cot, dong+chon), ToaDo(cot+rong, dong+cao*chon), str[chon], 14, 0);
				chon--;
				ColorBox(ToaDo(cot, dong+chon), ToaDo(cot+rong, dong+cao*chon), str[chon], 1, 15);
				SetBGColor(0);
			}break;
			case DOWN: if(chon+1 < so_item){
				ColorBox(ToaDo(cot, dong+chon), ToaDo(cot+rong, dong+cao*chon), str[chon], 14, 0);
				chon++;
				ColorBox(ToaDo(cot, dong+chon), ToaDo(cot+rong, dong+cao*chon), str[chon], 1, 15);
				SetBGColor(0);
			}break;
			case ENTER: {
				ColorBox(ToaDo(cot, dong+chon), ToaDo(cot+rong, dong+cao*chon), str[chon], 7, 5);
				SetBGColor(0);
				return chon+1;
			}
			case ESC: return -1;
		}
	}
}
void BoxNoiDung(string noidung[], int soitem, int x1, int y1, int x2, int y2){
	Box(ToaDo(x1-2, y1-1), ToaDo(x2, y2));
	const int bientren = y1-1;
	const int bienduoi = y2;
	int dong = y1, cot = x1;
	int i = 0, chon = 0;
	int trove = y2-y1-1;
	int flag = 0;
again:
	XoaNoiDung(y1, y2-y1, 0);
	Box(ToaDo(x1-2, y1-1), ToaDo(x2, y2));
	int demsodong = 0;
	for(i; i < soitem; i++){
		if(dong == bienduoi)  break;
		gotoxy(cot, dong);  cout<<noidung[i];
		dong++;
		demsodong++;// so dong dc hien thi;
	}	
	SetColor(11);  SetBGColor(0);
	if(flag == 0)  dong -= demsodong;
	else  dong = bienduoi-1;
	gotoxy(cot, dong);  cout<<noidung[chon];
	SetColor(15);  SetBGColor(0);
	while(true){
		char ch = getch();
		switch(ch){
			case UP: if(chon+1 > 1){
				SetColor(15);  SetBGColor(0);
				gotoxy(cot, dong);  cout<<noidung[chon];
				dong --;  chon--;
				
				if(dong == bientren){
					dong = y1; cot = x1;
					i = chon-trove;
					flag = 1;
					goto again;
				}
				SetColor(11);  SetBGColor(0);
				gotoxy(cot, dong);  cout<<noidung[chon];
			}break;
			case DOWN: if(chon+1 < soitem){
				SetColor(15);  SetBGColor(0);
				gotoxy(cot, dong);  cout<<noidung[chon];
				dong ++;  chon++;
				
				if(dong == bienduoi){
					i = chon;
					dong = y1;  cot = x1;
					flag = 0;
					goto again;
				}
				SetColor(11);  SetBGColor(0);
				gotoxy(cot, dong);  cout<<noidung[chon];
			}break;
			case ESC: {
				XoaNoiDung(7, 32, 0);
				return;
			}
		}
	}
}
//============CAC HAM KHAC====================================================================================
void swap(int &a, int &b){
	int tmp = a;
	a = b;
	b = tmp;
}

char *GetSystemDate(){
	time_t curTime = time(0);
	tm* now = localtime(&curTime);
	char *d = new char[11];
	sprintf(d, "%d/%d/%d", now->tm_mday, now->tm_mon+1, now->tm_year+1900);
	return d;
}
int* SplitDate(char s[]){
	int *a = new int[3];
	for(int i=0; i<3; i++) a[i] = -1;
	string txt;
	int index = 0;
	int n = strlen(s);
	for(int i=0; i<n; i++){
		if(s[i] == '/' || i == n-1){
			if(i == n-1) txt += s[i];
			int x = atoi(txt.c_str());
			a[index++] = x;
			txt = "";
		}else{
			txt += s[i];
		}
	}
	return a;
}
time_t GetTime(char *s){
	tm t;
	int *a = SplitDate(s);
	t.tm_year = a[2] - 1900;
	t.tm_mon = a[1] - 1;
	t.tm_mday = a[0];
	t.tm_hour = 0;
	t.tm_min = 0;
	t.tm_sec = 0;
	time_t timer = mktime(&t);
	delete a;
	return timer;
}
double DiffTime(char *time1, char *time2){
	time_t t1 = GetTime(time1);
	time_t t2 = GetTime(time2);
	return difftime(t1, t2);
}
int CurrentYear(){
	time_t curTime = time(0);
	tm* now = localtime(&curTime);
	return now->tm_year+1900;
}
bool IsNotNumber(string str){
	for (int i = 0; i < str.length(); i++){
		if(!isdigit(str[i]) || str[i] == ' ')
			return true;
	}
	return false;
}
string DinhDangChuoi(string str){
	int duoi = str.length()-1;
	while(str[duoi] == ' ')
		str.erase(duoi--, 1);
	int dau = 0;
	if(dau<duoi){
		while(str[dau] == ' ')
			dau++;
	}
	str = str.substr(dau, str.length());
	for(int i = dau; i < str.length(); i++){
		if(str[i]==' ' && str[i+1]==' '){
			str.erase(i, 1);
			i = 0;
		}
	}
	return str;
}
int GetKeyCode(){
    int ch = getch();
    if (ch == 0 || ch == 224)
        ch = 128 + getch();
    return ch;
}
void UserInput(int x, int y, int gioihan, string &user_str){
	const int ARROW_UP = 128+72;
	const int ARROW_DOWN = 128+80;
	const int ARROW_LEFT = 128+75;
	const int ARROW_RIGHT = 128+77;
	gotoxy(x, y);
	ShowCur(true);
	int ch, i = 0, tmp_int = x;
	while((ch = GetKeyCode()) != ENTER){
		if(ch != ARROW_UP && ch != ARROW_DOWN){
			if(ch == BACK_SPACE){
				if(tmp_int > x){
					string tmp_str = user_str.substr(i, user_str.length()-i);
					tmp_str = DinhDangChuoi(tmp_str);
					user_str.erase(i-1);
					user_str = user_str + tmp_str;
					ShowCur(false);
					for(int space = 0; tmp_int+space < gioihan-1; space++){
						gotoxy(tmp_int+space, y);
						cout<<" ";
					}
					if(tmp_str.length() == 0){
						gotoxy(tmp_int-1, y);
						cout<<" ";
					}
					else{
						gotoxy(tmp_int-1, y);
						cout<<tmp_str;
					}
					gotoxy(tmp_int-1, y);
					tmp_int--;
					i--;
					ShowCur(true);			
				}
			}
			else if(ch==32 || ch>=97 && ch<=122 || ch>=65 && ch<=90 || ch>=33 && ch<=57){
				string tmp_str = user_str.substr(i, user_str.length()-i);
				if(tmp_int+tmp_str.length() < gioihan-1){
					if(ch>=97 && ch<=122)	
					ch = ch-32; 
					user_str.insert(i, reinterpret_cast<const char*>(&ch));
					cout<<(char)ch;
					tmp_str = DinhDangChuoi(tmp_str);
					cout<<tmp_str;
					i++; 
					tmp_int++;
					gotoxy(tmp_int, y);
				}
			}
			else if(ch == ARROW_LEFT){
				if(tmp_int>x){
					tmp_int--;
					i--;
					gotoxy(tmp_int, y);
				}
			}
			else if(ch==ARROW_RIGHT){
				string tmp_str = user_str.substr(i, user_str.length());
				tmp_str = DinhDangChuoi(tmp_str);
				const int gioihandichuyen = tmp_str.length()+tmp_int;
				if(tmp_int < gioihandichuyen){
					tmp_int++;
					i++;
					gotoxy(tmp_int, y);
				}
			}	
		}
	}
	user_str = DinhDangChuoi(user_str);
	ShowCur(false);
	ColorBox(ToaDo(x, y-1), ToaDo(x+gioihan-2, y+1), "", 15, 0);
	gotoxy(x, y);
	cout<<user_str;
}
int madocgia[MAX_MADOCGIA];
int soluongdocgia = 0;
int vitridocgia = 0;
int LayMaDocGia(){
	return madocgia[vitridocgia++];
}
void TaoFileMaDocGia(){
	int MAX = 10000;
	int a[MAX];
	int i = 0, j = 1;
	while(i < MAX){
		a[i] = j;
		i++; j++;
	}
	i = 0;
	while(i<MAX_MADOCGIA){
		int numrand = rand() % (MAX-i+1) + (1+i);
		int tmp = a[numrand];
		a[numrand] = a[i];
		a[i] = tmp;
		i++;
	}
	fstream myfile;
	myfile.open("data/madocgia.txt", ios::out);
	for(i = 0; i< MAX_MADOCGIA; i++){
		myfile<<a[i]<<" ";
	}
}
void ReadMaDocGiaFromFile(int madocgia[]){
	fstream code;
	code.open("data/madocgia.txt", ios::in);
	if(!code.is_open()){
		cout<<"loi mo file";
		return;
	}
	int i = 0;
	while(code>>madocgia[i]){
		i++;
	}
	code.close();
}
void WriteMaDocGiaToFile(int madocgia[]){
	fstream code;
	code.open("data/madocgia.txt", ios::out | ios::trunc);
	if(!code.is_open()){
		cout<<"loi mo file";
		return;
	}
	for(int i = 0; i < MAX_MADOCGIA; i++){
		code << madocgia[i] << " ";
	}
	code.close();
}
void ReadDocGiaFromFile(NODE_DOCGIA* &TreeDG){
	fstream fileDocGia, fileMuonTra;
	fileDocGia.open("data/data_docgia.txt", ios::in);
	fileMuonTra.open("data/data_muontra.txt", ios::in);
	if(!fileDocGia.is_open() || !fileMuonTra.is_open()){
		cout<<"Loi mo file";
		return;
	}
	string line, lineMT;
	
	fileDocGia >> soluongdocgia;
	fileDocGia.ignore();/////////////////
	int sosachmuon;
	for(int i=0; i<soluongdocgia; i++){
		DOCGIA dg;
		
		getline(fileDocGia, line);		dg.MATHE = atoi(line.c_str());
		getline(fileDocGia, line);		dg.ho = line;
		getline(fileDocGia, line);		dg.ten = line;
		getline(fileDocGia, line);		dg.phai = line;
		getline(fileDocGia, line);		dg.trangthai = atoi(line.c_str());
		
		getline(fileMuonTra, lineMT);	sosachmuon = atoi(lineMT.c_str());
		for(int j=0; j<sosachmuon; j++){
			MUONTRA mt;
			getline(fileMuonTra, lineMT); 	mt.MASACH = lineMT;
			getline(fileMuonTra, lineMT);	strcpy(mt.NgayMuon, lineMT.c_str());
			getline(fileMuonTra, lineMT);	strcpy(mt.NgayTra, lineMT.c_str());
			getline(fileMuonTra, lineMT);	mt.trangthai = atoi(lineMT.c_str());
			InsertFirst_MuonTra(dg.dsmuontra, mt);
		}
		Insert_DocGia(TreeDG, dg);
	}
	vitridocgia = soluongdocgia;
	fileMuonTra.close();
	fileDocGia.close();
}
 
void WriteDocGiaToFile(NODE_DOCGIA* &TreeDG){
	if(TreeDG == NULL) return;
	 
	fstream fileDocGia, fileMuonTra;
	fileDocGia.open("data/data_docgia.txt", ios::out | ios::trunc);
	fileMuonTra.open("data/data_muontra.txt", ios::out | ios::trunc);

	int i=0;
	Queue<NODE_DOCGIA*> q;
	NODE_DOCGIA *node;
	NODE_MUONTRA *nodeMT;
	q.Insert_queue(TreeDG);
	
	fileDocGia << soluongdocgia << endl;
	while(!q.empty()){
		node = q.GetFront();	q.Delete_queue();
		
		fileDocGia << node->docgia.MATHE << endl;
		fileDocGia << node->docgia.ho << endl;
		fileDocGia << node->docgia.ten << endl;
		fileDocGia << node->docgia.phai << endl;
		fileDocGia << node->docgia.trangthai << endl;
		
		fileMuonTra << node->docgia.dsmuontra.total << endl;
		for(nodeMT = node->docgia.dsmuontra.First; nodeMT != NULL; nodeMT = nodeMT->next){
			fileMuonTra << nodeMT->data.MASACH << endl;
			fileMuonTra << nodeMT->data.NgayMuon << endl;
			fileMuonTra << nodeMT->data.NgayTra << endl;
			fileMuonTra << nodeMT->data.trangthai << endl;
		}
		if(node->left != NULL) q.Insert_queue(node->left);
		if(node->right != NULL) q.Insert_queue(node->right);
	}
	
	fileMuonTra.close();
	fileDocGia.close();
}
void ThanhPhanDocGia(DOCGIA d, bool isXoavaChinhsua = false){
	string ttdocgia[] = {"MA DOC GIA", "HO", "TEN", "GIOI TINH", "TRANG THAI","NAM", "KHOA", "NU", "HOAT DONG"};
	int x = 15;
	int y = 10;
	int rong = 15;
	for(int i=0; i<5; i++){
		ColorBox(ToaDo(x, y+4*i), ToaDo(x+rong, y+2+4*i), ttdocgia[i], 12, 15);
		if(i<=2)  ColorBox(ToaDo(x+rong+8, y+4*i), ToaDo(x+rong+8+70, y+2+4*i), "",15, 0);
		else{
			ColorBox(ToaDo(x+rong+8, y+4*i), ToaDo(x+rong+8+15, y+2+4*i),ttdocgia[i+2],15,0);
			ColorBox(ToaDo(x+rong+25, y+4*i), ToaDo(x+rong+25+15, y+2+4*i),ttdocgia[i+4],15,0);
		}
	}
	SetColor(0);
	SetBGColor(15);
	if(isXoavaChinhsua){
		gotoxy(40, 11); cout<<d.MATHE;
		gotoxy(40, 15); cout<<d.ho;
		gotoxy(40, 19); cout<<d.ten;
		if(d.phai == "NAM")  ColorBox(ToaDo(x+rong+8, y+4*3), ToaDo(x+rong+8+15, y+2+4*3),"NAM",1,0);
		else if(d.phai == "NU")  ColorBox(ToaDo(x+rong+25, y+4*3), ToaDo(x+rong+25+15, y+2+4*3),"NU",1,0);
		if(d.trangthai == 0)  ColorBox(ToaDo(x+rong+8, y+4*4), ToaDo(x+rong+8+15, y+2+4*4),"KHOA",1,0);
		else if(d.trangthai == 1)  ColorBox(ToaDo(x+rong+25, y+4*4), ToaDo(x+rong+25+15, y+2+4*4),"HOAT DONG",1,0);
	}
}
int ThemDocGia(DOCGIA &dgia){
	ThanhPhanDocGia(dgia);
	SetBGColor(15);
	SetColor(0);
	int madocgia = LayMaDocGia();
	gotoxy(40, 11);
	cout<<madocgia;
	dgia.MATHE = madocgia;
	while(dgia.ho.empty()){
		UserInput(40, 15, 70, dgia.ho);
		if(dgia.ho == "ESC"){
			dgia.ho.clear();
			return 0;
		}
	}
	while(dgia.ten.empty()){
		UserInput(40, 19, 70, dgia.ten);
		if(dgia.ten == "ESC"){
			dgia.ten.clear();
			return 0;
		}
	}
	int chon1 = 0;
	string phai[2] = {"NAM", "NU"};
	chon1 = MenuNgang(phai, 38, 22, 2);
	if(chon1 == 1)  dgia.phai = "NAM";
	else if(chon1 == 2) dgia.phai = "NU";
	else  return 0;
	int chon2 = 0;
	string trangthai[2] = {"KHOA", "HOAT DONG"};
	chon2 = MenuNgang(trangthai, 38, 26, 2);
	if(chon2 == 1)	dgia.trangthai = 0;
	else if(chon2 == 2)  dgia.trangthai = 1; 
	else  return 0;
	return 1;
}
void MenuChinhSuaDocGia(DOCGIA &dgia){
	string suadocgia[5] = {"HOAN TAT  ", "HO        ", "TEN       ", "GIOI TINH  ", "TRANG THAI"};
	ColorBox(ToaDo(48, 32), ToaDo(75, 34), "  HIEU CHINH", 8, 14);
	string tmp;
	int chon = 0;
	while(chon != 1){
		chon = MenuDoc(suadocgia, 48, 35, 5, 27, 1);
		SetColor(0);
		if(chon == 1){
			XoaNoiDung(8, 33, 0);
			break;
		}
		switch(chon){
			case 2:{
				SetBGColor(15);
				do{
					UserInput(40, 15, 70, dgia.ho);	
				}while(dgia.ho.empty());
				if(dgia.ho == "ESC")  dgia.ho.clear();
			}break;
			case 3:{
				SetBGColor(15);
				do{
					UserInput(40, 19, 70, dgia.ten);	
				}while(dgia.ten.empty());
				if(dgia.ten == "ESC")  dgia.ten.clear();
			}break;
			case 4:{
				int chonGT = 0;
				string phai[2] = {"NAM", "NU"};
				chonGT = MenuNgang(phai, 38, 22, 2);
				if(chonGT == 1)  dgia.phai = "NAM";
				else if(chonGT == 2)  dgia.phai = "NU";
			}break;
			case 5:{
				int chonTT = 0;
				string trangthai[2] = {"KHOA", "HOAT DONG"};
				chonTT = MenuNgang(trangthai, 38, 26, 2);
				if(chonTT == 1)	dgia.trangthai = 0;
				else if(chonTT == 2)  dgia.trangthai = 1;
			}break;
		}
	}
}
int XoaDocGia(){
	ColorBox(ToaDo(15, 10), ToaDo(30, 12), "MA DOC GIA", 4, 15);
	ColorBox(ToaDo(38, 10), ToaDo(108, 12), "",15, 0);
	string tmp;
	stringstream ss;
	while(true){
		//stringstream ss;
		do{
			UserInput(40, 11, 70, tmp);
		}while(tmp.empty());
		if(tmp == "ESC")  return -1;
		if(IsNotNumber(tmp)){
			MessageBox(GetConsoleWindow(), "Ma doc gia la so nguyen", "Thong bao!", MB_OK | MB_OK | MB_ICONINFORMATION);
			return -1;
		}
		int maDG;
		ss << tmp;
		ss >> maDG;
		NODE_DOCGIA *d = SearchDG(root, maDG);
		if(d != NULL){
			ThanhPhanDocGia(d->docgia, true);
			if(d->docgia.dsmuontra.chuatra > 0){
				MessageBox(GetConsoleWindow(), "Khong the xoa, doc gia nay dang muon sach!", "Thong bao!", MB_OK | MB_ICONINFORMATION);
				break;
			}
			int msg1 = MessageBox(GetConsoleWindow(), "Xoa doc gia nay?", "CAU HOI", MB_OKCANCEL | MB_ICONQUESTION);
			if(msg1 == IDOK){
				int msg2 = MessageBox(GetConsoleWindow(), "BAN CHAC CHAN MUON XOA DOC GIA NAY?", "CAU HOI", MB_OKCANCEL | MB_ICONQUESTION);
				if(msg2 == IDOK){
					RemoveDocGia(root, maDG);
					MessageBox(GetConsoleWindow(), "Da xoa doc gia!", "Thong bao!", MB_OK | MB_ICONINFORMATION);
					return maDG;
				}else break;
			}else  break;
		}else  MessageBox(GetConsoleWindow(), "Khong tim thay!", "Thong bao!", MB_OK | MB_ICONINFORMATION);
	}
	return -1;
}
void HieuChinhThongTinDG(){
	ColorBox(ToaDo(15, 10), ToaDo(30, 12), "MA DOC GIA", 4, 15);
	ColorBox(ToaDo(38, 10), ToaDo(108, 12), "",15, 0);
	string tmp;
	stringstream ss;
	do{
		UserInput(40, 11, 70, tmp);
	}while(tmp.empty());
	if(tmp == "ESC")  return;
	if(IsNotNumber(tmp)){
		MessageBox(GetConsoleWindow(), "Ma doc gia la so nguyen", "Thong bao!", MB_OK | MB_ICONINFORMATION);
		return;
	}
	int maDG;
	ss << tmp;
	ss >> maDG;
	NODE_DOCGIA *dgia = SearchDG(root, maDG);
	if(dgia != NULL){
		if(dgia->docgia.dsmuontra.chuatra > 0){
			MessageBox(GetConsoleWindow(), "Khong the hieu chinh, doc gia nay dang muon sach!", "Thong bao!", MB_OK | MB_ICONINFORMATION);
			return;
		}
		ThanhPhanDocGia(dgia->docgia, true);
		int msg = MessageBox(GetConsoleWindow(), "BAN CHAC CHAN MUON CHINH SUA DOC GIA NAY?", "CAU HOI", MB_OKCANCEL | MB_ICONQUESTION);
		if(msg == IDOK){
			MenuChinhSuaDocGia(dgia->docgia);
			MessageBox(GetConsoleWindow(), "Da sua thong tin doc gia!", "Thong bao!", MB_OK | MB_ICONINFORMATION);	
		}
	}
	else	MessageBox(GetConsoleWindow(), "Khong tim thay doc gia!", "Thong bao!", MB_OK | MB_ICONINFORMATION);
}
void HoTenDocGiaTangDan(){
	if(soluongdocgia == 0){
		MessageBox(GetConsoleWindow(), "Danh sach doc gia trong!", "Thong bao!", MB_OK | MB_ICONINFORMATION);
		return;
	}
	DOCGIA *d = new DOCGIA[soluongdocgia];
	InOrder(root, d, soluongdocgia);
	int i, j;
	DOCGIA x;
	for(i=1; i<soluongdocgia; i++){
		x = d[i];
		for(j=i-1; j>=0 && (x.ten+x.ho < d[j].ten+d[j].ho); j--)	   d[j+1] = d[j];
		d[j+1] = x;
	}
	ofstream file;
	file.open("data/tmp_docgia.txt", ios::out | ios::trunc);
	file << soluongdocgia << endl;
	for(int i=0; i<soluongdocgia; i++){
		file << d[i].MATHE << endl;
		file << d[i].ho << endl;
		file << d[i].ten << endl;
		file << d[i].phai << endl;
		file << d[i].trangthai << endl;
	}
	file.close();
	delete[] d;
}

void MaDocGiaTangDan(){
	if(soluongdocgia == 0){
		MessageBox(GetConsoleWindow(), "Danh sach doc gia trong!", "Thong bao!", MB_OK | MB_OK | MB_ICONINFORMATION);
		return;
	}
	DOCGIA *d = new DOCGIA[soluongdocgia];
	InOrder(root, d, soluongdocgia);
	ofstream file;
	file.open("data/tmp_docgia.txt", ios::out | ios::trunc);
	file << soluongdocgia << endl;
	for(int i=0; i<soluongdocgia; i++){
		file << d[i].MATHE << endl;
		file << d[i].ho << endl;
		file << d[i].ten << endl;
		file << d[i].phai << endl;
		file << d[i].trangthai << endl;
	}
	file.close();
	delete[] d;
}
int TotalPageDG = 1, CurentPageDG = 1;
void DrawBorderDSDocGia(int trang){
	SetColor(11);
	gotoxy(8, 8);     cout<<"MA DOC GIA";
	gotoxy(31, 8);    cout<<"HO";
	gotoxy(74, 8);    cout<<"TEN";
	gotoxy(89, 8);    cout<<"GIOI TINH";
	gotoxy(104, 8);   cout<<"TRANG THAI";
	gotoxy(57, 39);   cout<<"<--Trang "<<CurentPageDG<<"-->";
	SetColor(15);
	SetBGColor(0);
	OutChar(5, 9, 218);
	OutChar(20, 9, 194);
	OutChar(55, 9, 194);
	OutChar(85, 9, 194);
	OutChar(101, 9, 194);
	OutChar(117, 9, 191);
	
	OutChar(5, 11, 195);
	OutChar(20, 11, 197);
	OutChar(55, 11, 197);
	OutChar(85, 11, 197);
	OutChar(101, 11, 197);
	OutChar(117, 11, 180);
	Line(ToaDo(6, 9), 19, 0, 196);
	Line(ToaDo(21, 9), 54, 0, 196);
	Line(ToaDo(56, 9), 84, 0, 196);
	Line(ToaDo(86, 9), 100, 0, 196);
	Line(ToaDo(102, 9), 116, 0, 196);
	int tongsotrang = soluongdocgia/14 + 1;
	int sodong;
	if(soluongdocgia>=14 && trang<tongsotrang) sodong = 14;
	else sodong = soluongdocgia%14;
	for(int i=0; i<sodong && trang <=tongsotrang; i++){
		OutChar(5, 10+2*i, 179);
		OutChar(20, 10+2*i, 179);
		OutChar(55, 10+2*i, 179);
		OutChar(85, 10+2*i, 179);
		OutChar(101, 10+2*i, 179);
		OutChar(117, 10+2*i, 179);
		if(i<sodong-1){
			OutChar(5, 11+2*i, 195);
			OutChar(20, 11+2*i, 197);
			OutChar(55, 11+2*i, 197);
			OutChar(85, 11+2*i, 197);
			OutChar(101, 11+2*i, 197);
			OutChar(117, 11+2*i, 180);
		}
		else{
			OutChar(5, 11+2*i, 192);
			OutChar(20, 11+2*i, 193);
			OutChar(55, 11+2*i, 193);
			OutChar(85, 11+2*i, 193);
			OutChar(101, 11+2*i, 193);
			OutChar(117, 11+2*i, 217);
		}
		Line(ToaDo(6, 11+2*i), 19, 0, 196);
		Line(ToaDo(21, 11+2*i), 54, 0, 196);
		Line(ToaDo(56, 11+2*i), 84, 0, 196);
		Line(ToaDo(86, 11+2*i), 100, 0, 196);
		Line(ToaDo(102, 11+2*i), 116, 0, 196);
	}
	int y = 10+sodong*2;
	XoaNoiDung(y, 38-y, 0);
}
string CVTrangThaiDG(int trangthai){
	string khoa = "KHOA";
	string hoatdong = "HOAT DONG";
	return(trangthai == 0 ? khoa : hoatdong);
}
void DrawItemDocGia(DOCGIA &dg, int i){
	int cot = 6, hang = 10;
	i %= 14;
	gotoxy(7, hang+2*i); cout<<dg.MATHE;
	gotoxy(23, hang+2*i); cout<<dg.ho;
	gotoxy(59, hang+2*i); cout<<dg.ten;
	gotoxy(92, hang+2*i); cout<<dg.phai;
	gotoxy(103, hang+2*i); cout<<CVTrangThaiDG(dg.trangthai);
}
void XoaNoiDungDSDG(){
	for(int i=0; i<14; i++){
		gotoxy(7,10+2*i);     cout<<"           "; 
		gotoxy(23,10+2*i);    cout<<"                               "; 
		gotoxy(59,10+2*i);    cout<<"                          "; 
		gotoxy(92,10+2*i);    cout<<"   "; 
		gotoxy(103,10+2*i);   cout<<"         "; 
	}
}
void DrawDSDocGia(){
	if(soluongdocgia == 0){
		getch();
		return;
	}
	DOCGIA *d = new DOCGIA[soluongdocgia];
	ifstream file;
	file.open("data/tmp_docgia.txt", ios::in);
	string line;
	getline(file, line);
	for(int i=0; i<soluongdocgia; i++){
		getline(file, line);		d[i].MATHE = atoi(line.c_str());
		getline(file, line);		d[i].ho = line;
		getline(file, line);		d[i].ten = line;
		getline(file, line);		d[i].phai = line;
		getline(file, line);		d[i].trangthai = atoi(line.c_str());
	}
	file.close();
	TotalPageDG = (soluongdocgia-1)/14+1;
	int	ch;
	while(true){
		DrawBorderDSDocGia(CurentPageDG);
		for(int i=14*(CurentPageDG-1); i<14*CurentPageDG; i++){
			if(i >= soluongdocgia)  break;
			DrawItemDocGia(d[i], i);
		}
		ch = getch();
		if(ch == LEFT && CurentPageDG >1){
			CurentPageDG--;
			XoaNoiDungDSDG();
			gotoxy(61, 39);  cout<<"  "; //cap nhat so trang
		}
		else if(ch == RIGHT && CurentPageDG < TotalPageDG){
			CurentPageDG++;
			XoaNoiDungDSDG();
			gotoxy(61, 39);  cout<<"  "; //cap nhat so trang
		}
		else if(ch == ESC){
			CurentPageDG = 1;
			XoaNoiDung(8, 33, 0);
			return;
		}
	}
	file.close();
}


/////// Xu li dau sach
int ke = 1;
void DanhMaSachTuDong(DAUSACH* &dsach){
	for(int i=1; i<=dsach->soluong; i++){
		SACH s;
		s.masach = dsach->ISBN + "-" + to_string(i);
		s.trangthai = 0;
		s.vitri = "KE "+to_string(ke)+" NGAN "+to_string(i);
		InsertFirst_NodeSach(dsach->dms, s);
	}
	ke++;
}
void WriteDauSachToFile(DS_DAUSACH &DSDS){
	fstream fileds, filedms;
	fileds.open("data/dsds.txt", ios::out | ios::trunc);
	filedms.open("data/dms.txt", ios::out | ios::trunc);
	if(!fileds.is_open()  || !filedms.is_open()){
		cout<<"Loi mo file";
		return;
	}
	/*int i, j;
	for(i=1; i<DSDS.n; i++){
		for(j=i-1; j>=0 && (DSDS.nodes[i]->theloai+DSDS.nodes[i]->tensach < DSDS.nodes[j]->theloai+DSDS.nodes[j]->tensach); j--)  
			DSDS.nodes[j+1] = DSDS.nodes[j];
		DSDS.nodes[j+1] = DSDS.nodes[i];	
	}*/   //(phat sinh loi them dau sach)
	int i;
	for(i=1; i<DSDS.n; i++){
		DAUSACH* x = DSDS.nodes[i];
		int vitri = 0;
		while(DSDS.nodes[i]->theloai+DSDS.nodes[i]->tensach > DSDS.nodes[vitri]->theloai+DSDS.nodes[vitri]->tensach)  vitri++;
		for(int j=i; j>vitri; j--)  DSDS.nodes[j] = DSDS.nodes[j-1];
		DSDS.nodes[vitri] = x;	
	}
	fileds << DSDS.n <<endl;
	
	
	for(i=0; i<DSDS.n; i++){
		fileds << DSDS.nodes[i]->ISBN <<endl;
		fileds << DSDS.nodes[i]->theloai <<endl;
		fileds << DSDS.nodes[i]->tensach <<endl;
		fileds << DSDS.nodes[i]->tacgia <<endl;
		fileds << DSDS.nodes[i]->NXB <<endl;
		fileds << DSDS.nodes[i]->sotrang <<endl;
		
		filedms << DSDS.nodes[i]->soluotmuon <<endl;
		filedms << DSDS.nodes[i]->soluong << endl;
		for(NODE_SACH *nodesach = DSDS.nodes[i]->dms; nodesach != NULL; nodesach = nodesach->next){
			filedms << nodesach->sach.masach << endl;
			filedms << nodesach->sach.trangthai << endl;
			filedms << nodesach->sach.vitri << endl;
		}
	}
	fileds.close();
	filedms.close();
}
void ReadDauSachFromFile(DS_DAUSACH &DSDS){
	fstream fileds, filedms;
	
	fileds.open("data/dsds.txt", ios::in);
	filedms.open("data/dms.txt", ios::in);
	
	if(!fileds.is_open()  || !filedms.is_open()){
		cout<<"Loi mo file";
		return;
	}
	string line, linedms;
	DAUSACH dausach;
	
	int n;
	fileds >> n;
	fileds.ignore();

	for(int i=0; i<n; i++){
		DAUSACH *dausach = new DAUSACH;
		
		getline(fileds, line);  dausach->ISBN = line;
		getline(fileds, line);  dausach->theloai = line;
		getline(fileds, line);  dausach->tensach = line;
		getline(fileds, line);  dausach->tacgia = line;
		getline(fileds, line);  dausach->NXB = line;
		getline(fileds, line);  dausach->sotrang = line;
		
		getline(filedms, linedms);  dausach->soluotmuon = atoi(linedms.c_str());
		getline(filedms, linedms);  dausach->soluong = atoi(linedms.c_str());
		for(int j=0; j<dausach->soluong; j++){
			SACH sach;
			getline(filedms, linedms);  sach.masach = linedms;
			getline(filedms, linedms);  sach.trangthai = atoi(linedms.c_str());
			getline(filedms, linedms);  sach.vitri = linedms;
			InsertFirst_NodeSach(dausach->dms, sach);
		}
		ke++;
		AddDauSach(DSDS, dausach);
	}
	fileds.close();
	filedms.close();
}
int KichThuocISBN(string ISBN){
	int dem = 0;
	for(int i=0; i<ISBN.length(); i++){
		if(isdigit(ISBN[i]))	dem++;
		else return 0;
	}
	return dem;
}
void ThanhPhanDauSach(DAUSACH *dsach, bool chedochinhsua = false){
	string tpds[] = {"ISBN", "THE LOAI", "TEN SACH", "TAC GIA", "SO TRANG", "NXB", "SO LUONG"};
	int x = 5;
	int y = 10;
	int rong = 15;
	for(int i=0; i<7; i++){
		ColorBox(ToaDo(x, y+4*i), ToaDo(x+rong, y+2+4*i), tpds[i], 12, 15);
		ColorBox(ToaDo(x+rong+8, y+4*i), ToaDo(x+rong+8+60, y+2+4*i), "",15, 0);
	}
	SetColor(0);
	SetBGColor(15);
	if(chedochinhsua){
		gotoxy(30, 11); cout<<dsach->ISBN;
		gotoxy(30, 15); cout<<dsach->theloai;
		gotoxy(30, 19); cout<<dsach->tensach;
		gotoxy(30, 23); cout<<dsach->tacgia;
		gotoxy(30, 27); cout<<dsach->sotrang;
		gotoxy(30, 31); cout<<dsach->NXB;
		gotoxy(30, 35); cout<<dsach->soluong;
	}
}
//string soluongsach;
int ThemDauSach(DAUSACH *dsach){
	ThanhPhanDauSach(dsach);
	string soluongsach;
	stringstream nxb;
	stringstream sl;
	int num;
	do{
		UserInput(30, 11, 60, dsach->ISBN);
		if(dsach->ISBN == "ESC")	return 0;
		if(KichThuocISBN(dsach->ISBN) != 13)
			MessageBox(GetConsoleWindow(),"ISBN phai la chuoi 13 so", "Thong bao", MB_OK | MB_ICONWARNING);
		else if(TonTaiISBN(DSDS, dsach->ISBN))		MessageBox(GetConsoleWindow(),"Ma ISBN da ton tai","Thong bao", MB_OK | MB_ICONWARNING);
	}while(dsach->ISBN.empty() || KichThuocISBN(dsach->ISBN) != 13 || TonTaiISBN(DSDS, dsach->ISBN));
	do{
		UserInput(30, 15, 60, dsach->theloai);
		if(dsach->theloai == "ESC")  return 0;
	}while(dsach->theloai.empty());
	do{
		UserInput(30, 19, 60, dsach->tensach);
		if(dsach->tensach == "ESC")  return 0;
	}while(dsach->tensach.empty());
	do{
		UserInput(30, 23, 60, dsach->tacgia);
		if(dsach->tacgia == "ESC")  return 0;
	}while(dsach->tacgia.empty());
	do{
		UserInput(30, 27, 60, dsach->sotrang);
		if(dsach->sotrang == "ESC")  return 0;
		if(IsNotNumber(dsach->sotrang)) MessageBox(GetConsoleWindow(),"So trang la so tu nhien!!","Thong bao", MB_OK | MB_ICONWARNING);
	}while(dsach->sotrang.empty() || IsNotNumber(dsach->sotrang));
	do{
		UserInput(30, 31, 60, dsach->NXB);
		nxb << dsach->NXB;  nxb >> num;
		if(dsach->NXB == "ESC")  return 0;
		if(IsNotNumber(dsach->NXB) || num > CurrentYear())  MessageBox(GetConsoleWindow(),"Nam xuat ban la so tu nhien va khong lon hon nam hien tai!","Thong bao", MB_OK | MB_ICONWARNING);
			nxb.clear();
	}while(dsach->NXB.empty() || IsNotNumber(dsach->NXB) || num > CurrentYear());
	do{
		UserInput(30, 35, 60, soluongsach);
		if(soluongsach == "ESC")  return 0;
		if(IsNotNumber(soluongsach))  MessageBox(GetConsoleWindow(),"So luong sach la so tu nhien!","Thong bao", MB_OK | MB_ICONWARNING);
	}while(soluongsach.empty() || IsNotNumber(soluongsach));
	sl << soluongsach;  sl >> dsach->soluong;
	DanhMaSachTuDong(dsach);
	
	dsach->soluotmuon = 0;
	return 1;
}
void XoaDauSach(){
	if(DSDS.n == 0){
		MessageBox(GetConsoleWindow(), "Danh sach dau sach rong!", "Thong bao!", MB_OK | MB_OK | MB_ICONINFORMATION);
		return;
	}
	ColorBox(ToaDo(15, 10), ToaDo(30, 12), "ISBN", 4, 15);
	ColorBox(ToaDo(38, 10), ToaDo(108, 12), "",15, 0);
	string isbn;
	while(true){
		do{
			UserInput(40, 11, 60, isbn);
		}while(isbn.empty());
		if(isbn == "ESC")  return;
		if(IsNotNumber(isbn)){
			MessageBox(GetConsoleWindow(), "ISBN la so tu nhien!", "Thong bao!", MB_OK | MB_ICONINFORMATION);
			return;
		}
		if(KichThuocISBN(isbn) != 13){
			MessageBox(GetConsoleWindow(), "ISBN la so tu nhien gom 13 chu so!", "Thong bao!", MB_OK | MB_ICONINFORMATION);
			return;
		}
		if(TonTaiISBN(DSDS, isbn)){
			if(DauSachDuocMuon(DSDS, isbn)){
				 MessageBox(GetConsoleWindow(), "Khong the xoa, dau sach dang co doc gia muon!", "Thong bao!", MB_OK | MB_ICONINFORMATION);
				 break;
			}
			int msg1 = MessageBox(GetConsoleWindow(), "Xoa dau sach nay?", "Cau hoi", MB_OKCANCEL | MB_ICONQUESTION);
			if(msg1 = IDOK){
				int msg2 = MessageBox(GetConsoleWindow(), "BAN CHAC CHAN MUON XOA DAU SACH NAY?", "CAU HOI", MB_OKCANCEL | MB_ICONQUESTION);
				if(msg2 == IDOK){
					Delete_DauSach(DSDS, isbn);
					MessageBox(GetConsoleWindow(), "DA XOA!", "THONG BAO", MB_OK | MB_ICONINFORMATION);
					return;
				}else  break;
			}else break;
		}else  MessageBox(GetConsoleWindow(), "Khong tim thay!", "Thong bao!", MB_OK | MB_ICONINFORMATION);
	}
}
void MenuChinhSuaDauSach(DAUSACH* &dsach){
	string soluongsach;
	stringstream nxb, sl;
	int num;
	ColorBox(ToaDo(91, 10), ToaDo(118, 12), "  HIEU CHINH", 8, 14);
	int chon = 0;
	while(chon != 1){
		chon = MenuDoc(SuaDauSach, 91, 13, 8, 27, 1);
		SetColor(0);
		switch(chon){
			case 1: {
				XoaNoiDung(8, 33, 0);
				break;
			}
			case 2: {
				bool tontaiISBN;
				do{
					SetBGColor(15);
					UserInput(30, 11, 60, dsach->ISBN);
					if(dsach->ISBN == "ESC")  break;
					else if(KichThuocISBN(dsach->ISBN) != 13)
						MessageBox(GetConsoleWindow(),"ISBN phai la chuoi 13 so", "Thong bao", MB_OK | MB_ICONWARNING);
					tontaiISBN = false;
					for(int i = 0; i<DSDS.n; i++){
						if(dsach != DSDS.nodes[i] && dsach->ISBN == DSDS.nodes[i]->ISBN){
							MessageBox(GetConsoleWindow(),"Ma ISBN da ton tai","Thong bao", MB_OK | MB_ICONWARNING);
							tontaiISBN = true;
						}
					}
				}while(dsach->ISBN.empty() || KichThuocISBN(dsach->ISBN) != 13 || tontaiISBN == true);
			}break;
			case 3: {
				do{
					SetBGColor(15);
					UserInput(30, 15, 60, dsach->theloai);
				}while(dsach->theloai.empty());
			}break;
			case 4: {
				do{
					SetBGColor(15);
					UserInput(30, 19, 60, dsach->tensach);
				}while(dsach->tensach.empty());
			}break;
			case 5: {
				do{
					SetBGColor(15);
					UserInput(30, 23, 60, dsach->tacgia);
				}while(dsach->tacgia.empty());
			}break;
			case 6: {
				do{
					SetBGColor(15);
					UserInput(30, 27, 60, dsach->sotrang);
					if(IsNotNumber(dsach->sotrang))  MessageBox(GetConsoleWindow(),"So trang la mot so tu nhien!","Thong bao", MB_OK | MB_ICONWARNING);
				}while(dsach->sotrang.empty() || IsNotNumber(dsach->sotrang));
			}break;
			case 7: {
				do{
					SetBGColor(15);
					UserInput(30, 31, 60, dsach->NXB);
					nxb << dsach->NXB;  nxb >> num;
					nxb.clear();
					if(IsNotNumber(dsach->NXB) || num>CurrentYear())  MessageBox(GetConsoleWindow(),"Nam xuat ban phai la so va khong lon hon nam hien tai","Thong bao", MB_OK | MB_ICONWARNING);
				}while(dsach->NXB.empty() || IsNotNumber(dsach->NXB) || num>CurrentYear());
			}break;
			case 8: {
				do{
					SetBGColor(15);
					soluongsach = to_string(dsach->soluong);
					UserInput(30, 35, 60, soluongsach);
					if(IsNotNumber(soluongsach))  MessageBox(GetConsoleWindow(),"So trang la mot so tu nhien!","Thong bao", MB_OK | MB_ICONWARNING);
				}while(soluongsach.empty() || IsNotNumber(soluongsach));
				sl << soluongsach;  sl >> dsach->soluong;
				ClearDMS(dsach->dms);
				DanhMaSachTuDong(dsach);
			}break;
		}
	}
}
void HieuChinhThongTinDauSach(){
	if(DSDS.n == 0){
		MessageBox(GetConsoleWindow(), "Danh sach dau sach rong!", "Thong bao!", MB_OK | MB_OK | MB_ICONINFORMATION);
		return;
	}
	ColorBox(ToaDo(15, 10), ToaDo(30, 12), "ISBN", 4, 15);
	ColorBox(ToaDo(38, 10), ToaDo(108, 12), "",15, 0);
	string isbn;
	SetBGColor(15);
	while(true){
		do{
			UserInput(40, 11, 60, isbn);
		}while(isbn.empty());
		if(isbn == "ESC")  return;
		if(IsNotNumber(isbn)){
			MessageBox(GetConsoleWindow(), "ISBN la so tu nhien!", "Thong bao!", MB_OK | MB_ICONINFORMATION);
			return;
		}
		if(KichThuocISBN(isbn) != 13){
			MessageBox(GetConsoleWindow(), "ISBN la so tu nhien gom 13 chu so!", "Thong bao!", MB_OK | MB_ICONINFORMATION);
			return;
		}
		if(TonTaiISBN(DSDS, isbn)){
			if(DauSachDuocMuon(DSDS, isbn)){
				MessageBox(GetConsoleWindow(), "Khong the sua, sach cua dau sach nay da co doc gia muon!", "Thong bao!", MB_OK | MB_ICONINFORMATION);
				return;
			}
			// can cai thien: dau sach co doc gia muon -> co the hieu chinh (xoa dong 1706-1709)
			DAUSACH *dsach = new DAUSACH;
			TimDauSachBangISBN(DSDS, isbn, dsach);
			XoaNoiDung(7, 32, 0);
			ThanhPhanDauSach(dsach, true);
			int msg = MessageBox(GetConsoleWindow(), "BAN CHAC CHAN MUON SUA DAU SACH NAY?", "CAU HOI", MB_OKCANCEL | MB_ICONQUESTION);
			if(msg == IDOK){
				MenuChinhSuaDauSach(dsach);
				MessageBox(GetConsoleWindow(), "Da sua thong tin dau sach!", "Thong bao!", MB_OK | MB_ICONINFORMATION);	
				//WriteDauSachToFile(DSDS);
				break;
			}else  break;
			delete dsach;  dsach = NULL;
		}else  MessageBox(GetConsoleWindow(), "Khong tim thay!", "Thong bao!", MB_OK | MB_ICONINFORMATION);
	}
}
int CurentPageDS = 1;
void DrawBorderDSDauSach(int soluongdausach, int trang){
	ColorBox(ToaDo(15, 6), ToaDo(30, 8), "TU KHOA", 4, 15);
	ColorBox(ToaDo(38, 6), ToaDo(108, 8), "ENTER de tim kiem thong tin sach theo ten",15, 0);
	SetColor(11);
	SetBGColor(0);
	gotoxy(11, 10);     cout<<"ISBN";
	gotoxy(30, 10);    cout<<"TEN SACH";
	gotoxy(51, 10);    cout<<"SO TRANG";
	gotoxy(72, 10);    cout<<"TAC GIA";
	gotoxy(93, 10);    cout<<"NXB";
	gotoxy(103, 10);    cout<<"THE LOAI";
	gotoxy(28, 39);   cout<<"                                                                               ";
	gotoxy(57, 39);   cout<<"<--Trang "<<CurentPageDS<<"-->";
	
	SetColor(15);
	SetBGColor(0);
	OutChar(5, 11, 218);
	OutChar(20, 11, 194);
	OutChar(50, 11, 194);
	OutChar(60, 11, 194);
	OutChar(90, 11, 194);
	OutChar(98, 11, 194);
	OutChar(118, 11, 191);
	
	OutChar(5, 13, 195);
	OutChar(20, 13, 197);
	OutChar(50, 13, 197);
	OutChar(60, 13, 197);
	OutChar(90, 13, 197);
	OutChar(98, 13, 197);
	OutChar(118, 13, 180);

	int maxsodong = 13;
	Line(ToaDo(6, 11), 19, 0, 196);
	Line(ToaDo(21, 11), 49, 0, 196);
	Line(ToaDo(51, 11), 59, 0, 196);
	Line(ToaDo(61, 11), 89, 0, 196);
	Line(ToaDo(91, 11), 97, 0, 196);
	Line(ToaDo(99, 11), 117, 0, 196);
	int tongsotrang = soluongdausach/maxsodong + 1;
	int sodong;
	if(soluongdausach>=maxsodong && trang<tongsotrang) sodong = 13; 
	else sodong = soluongdausach%maxsodong;
	for(int i=0; i<sodong && trang <=tongsotrang; i++){
		OutChar(5, 12+2*i, 179);
		OutChar(20, 12+2*i, 179);
		OutChar(50, 12+2*i, 179);
		OutChar(60, 12+2*i, 179);
		OutChar(90, 12+2*i, 179);
		OutChar(98, 12+2*i, 179);
		OutChar(118, 12+2*i, 179);
		if(i<sodong-1){
			OutChar(5, 13+2*i, 195);
			OutChar(20, 13+2*i, 197);
			OutChar(50, 13+2*i, 197);
			OutChar(60, 13+2*i, 197);
			OutChar(90, 13+2*i, 197);
			OutChar(98, 13+2*i, 197);
			OutChar(118, 13+2*i, 180);
		}
		else{
			OutChar(5, 13+2*i, 192);
			OutChar(20, 13+2*i, 193);
			OutChar(50, 13+2*i, 193);
			OutChar(60, 13+2*i, 193);
			OutChar(90, 13+2*i, 193);
			OutChar(98, 13+2*i, 193);
			OutChar(118, 13+2*i, 217);
		}
		Line(ToaDo(6, 13+2*i), 19, 0, 196);
		Line(ToaDo(21, 13+2*i), 49, 0, 196);
		Line(ToaDo(51, 13+2*i), 59, 0, 196);
		Line(ToaDo(61, 13+2*i), 89, 0, 196);
		Line(ToaDo(91, 13+2*i), 97, 0, 196);
		Line(ToaDo(99, 13+2*i), 117, 0, 196);
	}
	int y = 12+sodong*2;
	XoaNoiDung(y, 38-y, 0);
}
void DrawItemDS(DAUSACH *dsach, int phantuthu_i){
	int cot = 6, hang = 12;
	int i = phantuthu_i;
	i %= 13;
	gotoxy(6, hang+2*i); cout<<dsach->ISBN;
	gotoxy(22, hang+2*i); cout<<dsach->tensach;
	gotoxy(52, hang+2*i); cout<<dsach->sotrang;
	gotoxy(62, hang+2*i); cout<<dsach->tacgia;
	gotoxy(92, hang+2*i); cout<<dsach->NXB;
	gotoxy(100, hang+2*i); cout<<dsach->theloai;
}
int MenuSearchDS(string td[], int so_item){
  int chon =0;
  int i; 
  int cot = 22;
  int dong  = 12;
  SetColor(4);
  gotoxy(22,dong+chon*2);
  cout << td[chon];
  char kytu;
	do {
	  kytu = getch();
	  if (kytu==0) kytu = getch();
	  switch (kytu) {
	    case UP :if (chon+1 >1)
	  			  {
	  			  	SetColor(15);
	              	gotoxy(cot,dong+chon*2);
	              	cout << td[chon];
	              	chon --;
	              	SetColor(4);
	              	gotoxy(cot,dong+chon*2);
	              	cout << td[chon];
	  			  }
	  			  break;
	  	case DOWN :if (chon+1 <so_item)
	  			  {
	  		        SetColor(15);
	              	gotoxy(cot,dong+chon*2);
	              	cout << td[chon];
	              	chon ++;
	              	SetColor(4);
	              	gotoxy(cot,dong+chon*2);
	              	cout << td[chon];
	  			  }
	  			  break;
	  	case ESC: return -1;
	  	case ENTER : return chon+1;
	  } 
	}while (true);
}
void SearchDauSach(){
	string tmp;
	string td[DSDS.n];
	int ListIndexDS[DSDS.n];
	int m = 0;
	SetColor(6);
	gotoxy(30, 39);   cout<<"13 LA SO KET QUA TOI DA TIM ThAY -  HAY NHAP TU KHOA GAN DUNG NHAT";
	ColorBox(ToaDo(38, 6), ToaDo(108, 8), "", 15, 0);
	do{
		UserInput(39, 7, 70, tmp);
		if(tmp == "ESC")  return ;
	}while(tmp.empty());
	for(int i=0;i<DSDS.n; i++){
		if(DSDS.nodes[i]->tensach.find(tmp) != -1){
			td[m] = DSDS.nodes[i]->tensach;
			ListIndexDS[m] = i;
			m++;
		}
	}
	if(m == 0){
		MessageBox(GetConsoleWindow(), "Khong tim thay dau sach", "Thong bao!", MB_OK | MB_OK | MB_ICONINFORMATION);
		return;
	}
	XoaNoiDung(9, 32, 0);
	DrawBorderDSDauSach(m, 1);
	SetColor(6);
	gotoxy(30, 39);   cout<<"13 LA SO KET QUA TOI DA TIM ThAY -  HAY NHAP TU KHOA GAN DUNG NHAT";
	ColorBox(ToaDo(38, 6), ToaDo(108, 8), tmp, 15, 0);
	i = 0;
	for(i=0; i<13; i++){
		if(i >= m)  break;
		SetBGColor(0);
		SetColor(15);
		DrawItemDS(DSDS.nodes[ListIndexDS[i]], i);
	}
	int chon = 0;
	chon = MenuSearchDS(td, m);
	if(chon == -1){
		XoaNoiDung(10, 31, 0);
		return;
	}
	XoaNoiDung(9, 27, 0);
	DrawBorderDSDauSach(1, 1);
	ColorBox(ToaDo(38, 6), ToaDo(108, 8), tmp, 15, 0);
	SetBGColor(0);  SetColor(15);
	DrawItemDS(DSDS.nodes[ListIndexDS[chon-1]], 0);
	XoaNoiDung(14, 27, 0);
	fstream file;
	file.open("data/thongtinsach.txt", ios::out | ios::trunc);
	if(!file.is_open()){
		cout<<"loi mo file";
		return;
	}
	int dem = 0;
	for(NODE_SACH *s = DSDS.nodes[ListIndexDS[chon-1]]->dms; s != NULL; s = s->next){
		file << s->sach.masach;
		for(int space = 0; space < 32-s->sach.masach.length(); space++)  file<<" ";
		file << CoverTrangThaiSach(s->sach.trangthai) << endl;
		dem++;
	}
	file.close();
	file.open("data/thongtinsach.txt", ios::in);
	string str[dem];
	for(i=0; i<dem; i++)  getline(file, str[i]);
	file.close();
	SetColor(14);
	gotoxy(45, 15); cout<<"MA SACH";
	gotoxy(69, 15); cout<<"TRANG THAI";
	SetColor(15);
	BoxNoiDung(str, dem, 35, 17, 88, 36);
}
void XoaNoiDungDSDS(){
	for(int i=0; i<13; i++){
		gotoxy(6,12+2*i);     cout<<"              "; 
		gotoxy(21,12+2*i);    cout<<"                             "; 
		gotoxy(51,12+2*i);    cout<<"         "; 
		gotoxy(61,12+2*i);    cout<<"                             "; 
		gotoxy(91,12+2*i);   cout<<"       "; 
		gotoxy(99,12+2*i);   cout<<"                  "; 
	}
}
void DrawDSDauSach(){
	if(DSDS.n == 0){
		MessageBox(GetConsoleWindow(), "Danh sach dau sach trong!", "Thong bao!", MB_OK | MB_OK | MB_ICONINFORMATION);
		return;
	}
	int TotalPageDS = (DSDS.n-1)/13+1;
	int	ch;
	while(true){
		DrawBorderDSDauSach(DSDS.n, CurentPageDS);
		for(int i=13*(CurentPageDS-1); i<13*CurentPageDS; i++){
			if(i >= DSDS.n)  break;
			DrawItemDS(DSDS.nodes[i], i);
		}
		ch = getch();
		if(ch == LEFT && CurentPageDS >1){
			CurentPageDS--;
			XoaNoiDungDSDS();
			gotoxy(61, 39);  cout<<"  ";
		}
		else if(ch == RIGHT && CurentPageDS < TotalPageDS){
			CurentPageDS++;
			XoaNoiDungDSDS();
			gotoxy(61, 39);  cout<<"  ";
		}
		else if(ch == ENTER){
			SearchDauSach();
		}
		else if(ch == ESC){
			CurentPageDS = 1;
			XoaNoiDung(3, 38, 0);
			return;
		}
	}
}
void DrawMuonTra(){
	ColorBox(ToaDo(15, 2), ToaDo(30, 4), "MA DOC GIA", 4, 15);
	ColorBox(ToaDo(38, 2), ToaDo(108, 4), "",15, 0);
	SetBGColor(0);
	SetColor(9);
	Box(ToaDo(20, 7), ToaDo(103, 22));
	SetBGColor(0);
	SetColor(11);
	gotoxy(24, 10);  cout<<"   DOC GIA:";
	gotoxy(24, 13);  cout<<"    MA THE:";
	gotoxy(24, 16);  cout<<"      PHAI:";
	gotoxy(24, 19);  cout<<"TRANG THAI:";
	ColorBox(ToaDo(46, 24), ToaDo(77, 26), " SACH DOC GIA DANG MUON", 4, 14);
	SetBGColor(0);
	SetColor(15);
	gotoxy(20,29);  cout<<"MA SACH";
	gotoxy(57,29);  cout<<"TEN SACH";
	gotoxy(95,29);  cout<<"NGAY MUON";
	SetBGColor(0);
	SetColor(15);
	OutChar(15, 28, 218);
	OutChar(31, 28, 194);
	OutChar(90, 28, 194);
	OutChar(108, 28, 191);
	OutChar(15, 30, 195);
	OutChar(31, 30, 197);
	OutChar(90, 30, 197);
	OutChar(108, 30, 180);
	Line(ToaDo(16, 28), 30, 0, 196);
	Line(ToaDo(32, 28), 89, 0, 196);
	Line(ToaDo(91, 28), 107, 0, 196);
	for(int i=0; i<4; i++){
		OutChar(15, 29+2*i, 179);
		OutChar(31, 29+2*i, 179);
		OutChar(90, 29+2*i, 179);
		OutChar(108, 29+2*i, 179);
		if(i<4-1){
			OutChar(15, 30+2*i, 195);
			OutChar(31, 30+2*i, 197);
			OutChar(90, 30+2*i, 197);
			OutChar(108, 30+2*i, 180);
		}
		else{
			OutChar(15, 30+2*i, 192);
			OutChar(31, 30+2*i, 193);
			OutChar(90, 30+2*i, 193);
			OutChar(108, 30+2*i, 217);
		}
		Line(ToaDo(16, 30+2*i), 30, 0, 196);
		Line(ToaDo(32, 30+2*i), 89, 0, 196);
		Line(ToaDo(91, 30+2*i), 107, 0, 196);
	}
}
void DrawMuonTra2(bool IsMuonSach = true){
	ColorBox(ToaDo(15, 4), ToaDo(30, 6), "MA SACH", 4, 14);
	ColorBox(ToaDo(38, 4), ToaDo(108, 6), "",15, 0);
	SetBGColor(0);
	SetColor(9);
	Box(ToaDo(20, 9), ToaDo(103, 24));
	SetColor(11);
	gotoxy(24, 12);  cout<<"     MA SACH:";
	gotoxy(24, 15);  cout<<"    TEN SACH:";
	gotoxy(24, 18);  cout<<"  TRANG THAI:";
	gotoxy(24, 21);  cout<<"      VI TRI:";
	if(IsMuonSach)  ColorBox(ToaDo(35, 27), ToaDo(48, 29), "NGAY MUON", 4, 14);
	else ColorBox(ToaDo(35, 27), ToaDo(48, 29), "NGAY TRA", 4, 14);
	ColorBox(ToaDo(50, 27), ToaDo(88, 29), "",15, 0);
	char currdate[11];
	strcpy(currdate, GetSystemDate());
	SetBGColor(15);
	SetColor(0);
	gotoxy(65, 28);  cout<<currdate;
}
void ThongTinTopMuonTra(){
	bool IsNull = true;
	TOPSACH topsach(DSDS, IsNull);
	if(IsNull){
		MessageBox(GetConsoleWindow(), "Chua co sach nao duoc muon!", "Thong bao!", MB_OK | MB_OK | MB_ICONINFORMATION);
		XoaNoiDung(6, 32, 0);
		return;
	}
	for(int i = 0; i < (topsach.n < 10 ? topsach.n : 10); i++){
		gotoxy(8, 14+2*i);   cout<<DSDS.nodes[topsach.list[i].indexDS]->ISBN;
		gotoxy(23, 14+2*i);  cout<<DSDS.nodes[topsach.list[i].indexDS]->tensach;
		gotoxy(59, 14+2*i);  cout<<DSDS.nodes[topsach.list[i].indexDS]->sotrang;
		gotoxy(69, 14+2*i);  cout<<DSDS.nodes[topsach.list[i].indexDS]->tacgia;
		gotoxy(93, 14+2*i);  cout<<DSDS.nodes[topsach.list[i].indexDS]->NXB;
		gotoxy(100, 14+2*i); cout<<DSDS.nodes[topsach.list[i].indexDS]->theloai;
		gotoxy(114, 14+2*i); cout<<DSDS.nodes[topsach.list[i].indexDS]->soluotmuon;
	}
}
void DrawTop10MuonTra(){
	ColorBox(ToaDo(33, 6), ToaDo(90, 8), "TOP 10 SACH CO LUOT MUON NHIEU NHAT", 8, 15);
	SetColor(15);
	SetBGColor(0);
	gotoxy(3, 12); cout<<"STT";
	gotoxy(12, 12); cout<<"ISBN";
	gotoxy(37, 12); cout<<"TEN SACH";
	gotoxy(59, 12); cout<<"SO TRANG";
	gotoxy(76, 12); cout<<"TAC GIA";
	gotoxy(94, 12); cout<<"NXB";
	gotoxy(102, 12); cout<<"THE LOAI";
	gotoxy(114, 12); cout<<"SL.MUON";
	//STT
	for(int i=0; i<10; i++){
		if(i == 9) {
			gotoxy(3, 14+2*i); 
			cout<<i+1;
		}else{
			gotoxy(4, 14+2*i); 
			cout<<i+1;
		}
	}
	OutChar(2, 13, 218);
	OutChar(6, 13, 194);
	OutChar(22, 13, 194);
	OutChar(58, 13, 194);
	OutChar(68, 13, 194);
	OutChar(92, 13, 194);
	OutChar(98, 13, 194);
	OutChar(113, 13, 194);
	OutChar(121, 13, 191);

	OutChar(2, 15, 195);
	OutChar(6, 15, 197);
	OutChar(22, 15, 197);
	OutChar(58, 15, 197);
	OutChar(68, 15, 197);
	OutChar(92, 15, 197);
	OutChar(98, 15, 197);
	OutChar(113, 15, 197);
	OutChar(121, 15, 180);

	Line(ToaDo(3, 13), 5, 0, 196);
	Line(ToaDo(7, 13), 21, 0, 196);
	Line(ToaDo(23, 13), 57, 0, 196);
	Line(ToaDo(59, 13), 67, 0, 196);
	Line(ToaDo(69, 13), 91, 0, 196);
	Line(ToaDo(93, 13), 97, 0, 196);
	Line(ToaDo(99, 13), 112, 0, 196);
	Line(ToaDo(114, 13), 120, 0, 196);
	for(int i=0; i<10; i++){
		OutChar(2, 14+2*i, 179);
		OutChar(6, 14+2*i, 179);
		OutChar(22, 14+2*i, 179);
		OutChar(58, 14+2*i, 179);
		OutChar(68, 14+2*i, 179);
		OutChar(92, 14+2*i, 179);
		OutChar(98, 14+2*i, 179);
		OutChar(113, 14+2*i, 179);
		OutChar(121, 14+2*i, 179);
		if(i<10-1){
			OutChar(2, 15+2*i, 195);
			OutChar(6, 15+2*i, 197);
			OutChar(22, 15+2*i, 197);
			OutChar(58, 15+2*i, 197);
			OutChar(68, 15+2*i, 197);
			OutChar(92, 15+2*i, 197);
			OutChar(98, 15+2*i, 197);
			OutChar(113, 15+2*i, 197);
			OutChar(121, 15+2*i, 180);
		}
		else{
			OutChar(2, 15+2*i, 192);
			OutChar(6, 15+2*i, 193);
			OutChar(22, 15+2*i, 193);
			OutChar(58, 15+2*i, 193);
			OutChar(68, 15+2*i, 193);
			OutChar(92, 15+2*i, 193);
			OutChar(98, 15+2*i, 193);
			OutChar(113, 15+2*i, 193);
			OutChar(121, 15+2*i, 217);
		}
		Line(ToaDo(3, 15+2*i), 5, 0, 196);
		Line(ToaDo(7, 15+2*i), 21, 0, 196);
		Line(ToaDo(23, 15+2*i), 57, 0, 196);
		Line(ToaDo(59, 15+2*i), 67, 0, 196);
		Line(ToaDo(69, 15+2*i), 91, 0, 196);
		Line(ToaDo(93, 15+2*i), 97, 0, 196);
		Line(ToaDo(99, 15+2*i), 112, 0, 196);
		Line(ToaDo(114, 15+2*i), 120, 0, 196);
	}
	ThongTinTopMuonTra();
	while(true){
		int ch = getch();
		if(ch == ESC){
			XoaNoiDung(6, 32, 0);
			return;
		}
	}
}
int DrawSachDangMuon(MUONTRA *danhsachmuontra, int sosachdangmuon){
	int signal = 1;
	for(int i=0; i < sosachdangmuon; i++){
		string tensach;
		for(int vitri = 0; vitri < DSDS.n; vitri++){
			bool timthaysach = false;
			for(NODE_SACH *p = DSDS.nodes[vitri]->dms; p != NULL; p = p->next){
				if(p->sach.masach == danhsachmuontra[i].MASACH){
					tensach = DSDS.nodes[vitri]->tensach;
					timthaysach = true;
					break;
				}
			}
			if(timthaysach)  break;
		}
		SetColor(15);
		SetBGColor(0);
		gotoxy(16, 31+2*i);  cout << danhsachmuontra[i].MASACH << endl;
		gotoxy(34, 31+2*i);  cout << tensach << endl;
		gotoxy(95, 31+2*i);  cout << danhsachmuontra[i].NgayMuon << endl;
	}
	for(int i = 0; i < sosachdangmuon; i++){
		if(DiffTime(GetSystemDate(), danhsachmuontra[i].NgayMuon) > (7*24*60*60)){
			signal = 0;
			break;
		}
	}
	return signal;
}
void UpdateSach(string masach, int trangthai){
	for(int i = 0; i<DSDS.n; i++){
		for(NODE_SACH *p = DSDS.nodes[i]->dms; p != NULL; p=p->next){
			if(p->sach.masach == masach){
				p->sach.trangthai = trangthai;
				if(trangthai == 1)  DSDS.nodes[i]->soluotmuon++;
				return;
			}
		}
	}
}
void MenuMuonSach(){
	string tmp;
	stringstream ss;
	SetColor(0);
	SetBGColor(15);
	do{
		UserInput(40, 3, 70, tmp);
	}while(tmp.empty());
	if(tmp == "ESC")  return ;
	if(IsNotNumber(tmp)){
		MessageBox(GetConsoleWindow(), "MA DOC GIA LA 1 SO NGUYEN", "THONG BAO", MB_OK | MB_ICONINFORMATION);
		return;
	}
	ss << tmp;	int maDG;  ss >> maDG;
	NODE_DOCGIA *d = SearchDG(root, maDG);
	if(d != NULL){
		int sosachdangmuon = d->docgia.dsmuontra.chuatra;
		int flag = 1;
		if(sosachdangmuon == 0)  MessageBox(GetConsoleWindow(), "Doc  gia chua muon sach nao!", "Thong bao!", MB_OK | MB_OK | MB_ICONINFORMATION);
		else if(sosachdangmuon > 0 && sosachdangmuon <=3){
			MUONTRA *dsmuontra = new MUONTRA[sosachdangmuon];
			Traverse_MuonTra(d->docgia.dsmuontra, dsmuontra, 0);
			flag = DrawSachDangMuon(dsmuontra, sosachdangmuon);
		}
		if(sosachdangmuon >= 3){
			MessageBox(GetConsoleWindow(), "Doc gia da muon toi da 3 sach, vui long tra sach roi muon lai!", "Thong bao!", MB_OK | MB_OK | MB_ICONINFORMATION);
			return;
		}
		if(flag == 0 && sosachdangmuon != 0){
			MessageBox(GetConsoleWindow(), "Doc gia muon sach qua han, vui long tra sach roi muon lai!", "Thong bao!", MB_OK | MB_OK | MB_ICONINFORMATION);
			return;
		}
		SetColor(11);
		SetBGColor(0);
		gotoxy(45, 10);  cout << d->docgia.ho +" "+ d->docgia.ten;
		gotoxy(45, 13);  cout << d->docgia.MATHE;
		gotoxy(45, 16);  cout << d->docgia.phai;
		gotoxy(45, 19);  cout << CVTrangThaiDG(d->docgia.trangthai);
		gotoxy(39, 39);  cout<<"==>BAM PHIM BAT KI DE TIEN HANH MUON SACH <==";
		if(d->docgia.trangthai == 0){
			MessageBox(GetConsoleWindow(), "Doc gia da bi khoa", "Thong bao!", MB_OK | MB_OK | MB_ICONINFORMATION);
			XoaNoiDung(1, 40, 0);
			return;
		}
		while(true){
			int ch = getch();
			if(ch != ESC){
				XoaNoiDung(1, 40, 0);	DrawMuonTra2();
				SetColor(0);	SetBGColor(15); 	string _masach;
				while(_masach.empty()){
					while(_masach.empty()){
						UserInput(40, 5, 70, _masach);
					}
					if(_masach == "ESC")  return;
					string tensach;
					bool datimthay = false;
					for(int i = 0; i < DSDS.n; i++){
						for(NODE_SACH *p = DSDS.nodes[i]->dms; p != NULL; p = p->next){
							if(p->sach.masach == _masach){
								tensach = DSDS.nodes[i]->tensach;
								datimthay = true;	break;
							}
						}
						if(datimthay == true) break;
					}
					if(datimthay == false){
						ColorBox(ToaDo(38, 4), ToaDo(108, 6), "Bam phim bat ki de nhap lai ma sach",15, 0);
						MessageBox(GetConsoleWindow(), "Ma sach khong ton tai", "Thong bao!", MB_OK | MB_OK | MB_ICONINFORMATION);
						continue;					
					}				
					DAUSACH *dsach = new DAUSACH;
					TimDauSachBangTen(DSDS, tensach, dsach);
					string vitrisach;
					int trangthaisach;
					bool dacodocgiamuon = false;
					for(NODE_SACH *p = dsach->dms; p!=NULL; p = p->next){
						if(p->sach.masach == _masach && p->sach.trangthai == 1)
						{
							dacodocgiamuon = true;
							vitrisach = p->sach.vitri;
							trangthaisach = p->sach.trangthai;
							break;
						}
						if(p->sach.masach == _masach){
							vitrisach = p->sach.vitri;
							trangthaisach = p->sach.trangthai;
							break;
						}
					}
					SetColor(11);
					SetBGColor(0);
					gotoxy(45, 12); cout<<_masach;
					gotoxy(45, 15); cout<<tensach;
					gotoxy(45, 18); cout << CoverTrangThaiSach(trangthaisach);
					gotoxy(45, 21); cout<<vitrisach;
					if(dacodocgiamuon){
						MessageBox(GetConsoleWindow(), "Da co doc gia muon sach nay!", "Thong bao!", MB_OK | MB_OK | MB_ICONINFORMATION);
						return;
					}
					ColorBox(ToaDo(46, 34), ToaDo(77, 36), "MUON SACH | ENTER", 1, 14);
					while(true){
						int c = getch();
						if(c == ENTER){
							MUONTRA mt;
							mt.MASACH = _masach;
							strcpy(mt.NgayMuon, GetSystemDate());
							strcpy(mt.NgayTra, "");
							mt.trangthai = 0; //sach dang muon chua tra
							InsertFirst_MuonTra(d->docgia.dsmuontra, mt);
							UpdateSach(_masach, 1);
							MessageBox(GetConsoleWindow(), "Da muon sach thanh cong!", "Thong bao", MB_OK | MB_ICONINFORMATION);
							delete dsach;
							return;
						}
						else if(c == ESC)  return;
					}
				}
			}else return;
		}
	}else
		MessageBox(GetConsoleWindow(), "Khong tim thay!", "Thong bao", MB_OK | MB_ICONINFORMATION);
}
void UpdateDocGia(NODE_DOCGIA * &TreeDG, DOCGIA &docgia){
	NODE_DOCGIA *p = SearchDG(TreeDG, docgia.MATHE);
	if(p == NULL)  cout<<"Khong the update";
	else{
		docgia.dsmuontra = p->docgia.dsmuontra;
		p->docgia = docgia;
	}
}

void MenuTraSach(){
	string tmp;  stringstream ss;	SetColor(0);  SetBGColor(15);
	do{
		UserInput(40, 3, 70, tmp);
	}while(tmp.empty());
	if(tmp == "ESC")  return ;
	if(IsNotNumber(tmp)){
		MessageBox(GetConsoleWindow(), "MA DOC GIA LA 1 SO NGUYEN", "THONG BAO", MB_OK | MB_OK | MB_ICONINFORMATION);	return;
	}
	ss << tmp;  int maDG;  ss >> maDG;
	NODE_DOCGIA *d = SearchDG(root, maDG);
	if(d != NULL){
		int sosachdangmuon = d->docgia.dsmuontra.chuatra;// luu vao node muontra va tra ve tong
		if(sosachdangmuon == 0){
			MessageBox(GetConsoleWindow(), "Doc gia chua muon sach nao!", "Thong bao!", MB_OK | MB_OK | MB_ICONINFORMATION);
			return;
		}
		MUONTRA dsmuontra[sosachdangmuon];
		Traverse_MuonTra(d->docgia.dsmuontra, dsmuontra, 0);
		DrawSachDangMuon(dsmuontra, sosachdangmuon);
		SetColor(11);	SetBGColor(0);
		gotoxy(45, 10);  cout << d->docgia.ho +" "+ d->docgia.ten;
		gotoxy(45, 13);  cout << d->docgia.MATHE;
		gotoxy(45, 16);  cout << d->docgia.phai;
		gotoxy(45, 19);  cout << CVTrangThaiDG(d->docgia.trangthai);
		gotoxy(41, 39);  cout<<"Bam phim bat ki de tien hanh tra sach";
		if(d->docgia.trangthai == 0){
			MessageBox(GetConsoleWindow(), "Doc gia da bi khoa", "Thong bao!", MB_OK | MB_OK | MB_ICONINFORMATION);
			XoaNoiDung(1, 40, 0);	return;
		}
		while(true){
			int ch = getch();
			if(ch != ESC){
				XoaNoiDung(1, 40, 0);	DrawMuonTra2(false);	SetColor(0);	SetBGColor(15);
				string _masach;
				while(_masach.empty()){
					while(_masach.empty()){
						UserInput(40, 5, 70, _masach);
					}
					if(_masach == "ESC")  return;
					string tensach; 	bool datimthay = false;
					for(int i = 0; i < DSDS.n; i++){
						for(NODE_SACH *p = DSDS.nodes[i]->dms; p != NULL; p = p->next){
							if(p->sach.masach == _masach){
								tensach = DSDS.nodes[i]->tensach;
								datimthay = true;	break;
							}
						}
						if(datimthay == true) break;
					}
					if(datimthay == false){
						ColorBox(ToaDo(38, 4), ToaDo(108, 6), "Bam phim bat ki de nhap lai ma sach",15, 0);
						MessageBox(GetConsoleWindow(), "Ma sach khong ton tai", "Thong bao!", MB_OK | MB_OK | MB_ICONINFORMATION);
						continue;					
					}				
					DAUSACH *dsach = new DAUSACH;
					TimDauSachBangTen(DSDS,tensach, dsach);
					string vitrisach;
					int trangthaisach;
					for(NODE_SACH *p = dsach->dms; p!=NULL; p = p->next){
						if(p->sach.masach == _masach){
							vitrisach = p->sach.vitri;
							trangthaisach = p->sach.trangthai;  break;
						}
					}
					SetColor(11);	SetBGColor(0);
					gotoxy(45, 12); cout<<_masach;		gotoxy(45, 15);	cout<<tensach;		gotoxy(45, 18);	cout<<CoverTrangThaiSach(trangthaisach);	gotoxy(45, 21); cout<<vitrisach;
					string option[2] = {" LAM MAT SACH", " TRA SACH"};
					int chon = MenuNgang(option, 45, 34, 2);
					bool success = false;
					if(chon == 1){
						if(d->docgia.dsmuontra.First == NULL)  success = false;
						else{
							for(NODE_MUONTRA *mt = d->docgia.dsmuontra.First; mt != NULL; mt = mt->next){
								if(mt->data.MASACH == _masach  && mt->data.trangthai == 0){
									success = true;  break;
								}
							}
						}
						if(success){
							int thongbao = MessageBox(GetConsoleWindow(), "Da xac nhan doc gia da tra het sach chua mat?", "Cau hoi", MB_OKCANCEL | MB_ICONQUESTION);
							if(thongbao == IDOK){
								for(NODE_MUONTRA *mt = d->docgia.dsmuontra.First; mt != NULL; mt = mt->next){
									if(mt->data.MASACH == _masach  && mt->data.trangthai == 0){
										mt->data.trangthai = 2;
										UpdateSach(_masach, 2);
										d->docgia.trangthai = 0;
										UpdateDocGia(root, d->docgia);
									}
								}
								MessageBox(GetConsoleWindow(), "Da bao mat sach va khoa doc gia thanh cong!", "Thong bao!", MB_OK | MB_ICONINFORMATION);
								delete dsach;	return;
							}else{
								MessageBox(GetConsoleWindow(), "Hay tra het sach chua mat!", "Thong bao!", MB_OK | MB_ICONINFORMATION);
								delete dsach;	return;
							}
						}else{
							ColorBox(ToaDo(38, 4), ToaDo(108, 6), "Bam phim bat ki de nhap lai ma sach", 15, 0);
							MessageBox(GetConsoleWindow(), "Doc gia chua muon sach nay!", "Thong bao", MB_OK | MB_ICONINFORMATION);
							delete dsach;  break;
						}
					}
					if(chon == 2){
						char ngaytra[11];  strcpy(ngaytra, GetSystemDate());
						if(d->docgia.dsmuontra.First == NULL)  success = false;
						else{
							for(NODE_MUONTRA *mt = d->docgia.dsmuontra.First; mt != NULL; mt = mt->next){
								if(mt->data.MASACH == _masach  && mt->data.trangthai == 0){
									mt->data.trangthai = 1;
									strcpy(mt->data.NgayTra, ngaytra);
									UpdateSach(_masach, 0);
									d->docgia.dsmuontra.chuatra--;
									success = true;
									MessageBox(GetConsoleWindow(), "Da tra sach!", "Thong bao", MB_OK | MB_ICONINFORMATION);
									delete dsach;
									break;
								}
								else success = false;
							}
						}
						if(success ==  true){
							return;
						}else{
							ColorBox(ToaDo(38, 4), ToaDo(108, 6), "Bam phim bat ki de nhap lai ma sach",15, 0);
							MessageBox(GetConsoleWindow(), "Doc gia chua muon sach nay!", "Thong bao", MB_OK | MB_ICONINFORMATION);
							delete dsach; break;
						}	
					}else{	
						delete dsach;  return;
					}
				}
			}else break;
		}
	}else	MessageBox(GetConsoleWindow(), "Khong tim thay!", "Thong bao", MB_OK | MB_ICONINFORMATION);
}
void DrawMuonSachQuaHan(int songayqh[], int madocgiamuonqh[], int soluongdocgiaquahan){
	SetBGColor(0);
	SetColor(15);
	fstream file;
	file.open("data/danhsachdocgiaquahan.txt", ios::out | ios::trunc);
	if(!file.is_open()){
		cout<<"loi mo file";
		return;
	}
	int dem = 0;
	for(int i=0; i <= soluongdocgiaquahan; i++){
		NODE_DOCGIA *dgia = SearchDG(root, madocgiamuonqh[i]);
		if(dgia == NULL)  continue;
		string mathe = to_string(dgia->docgia.MATHE);
		int space;
		file << mathe;
		for(space = 0; space < 15 - mathe.length(); space++)
			file << " ";
		file << dgia->docgia.ho;
		for(space = 0; space < 32 - dgia->docgia.ho.length(); space++)
			file << " ";
		file << dgia->docgia.ten;
		for(space = 0; space < 28 - dgia->docgia.ten.length(); space++)
			file << " ";
		file << dgia->docgia.phai;
		for(space = 0; space < 13 - dgia->docgia.phai.length(); space++)
			file << " ";
		file << CVTrangThaiDG(dgia->docgia.trangthai);
		for(space = 0; space < 19 - CVTrangThaiDG(dgia->docgia.trangthai).length(); space++)
			file << " ";
		file << songayqh[i] << endl;
		dem++;
		if(dem >= soluongdocgiaquahan)  break;
	}
	file.close();
	file.open("data/danhsachdocgiaquahan.txt", ios::in);
	string str[dem];
	for(int i=0; i<dem; i++)  getline(file, str[i]);
	file.close();
	gotoxy(5, 9);  cout<<"ISBN";
	gotoxy(20, 9); cout<<"HO";
	gotoxy(52, 9); cout<<"TEN";
	gotoxy(77, 9); cout<<"GIOI TINH";
	gotoxy(92, 9); cout<<"TRANG THAI";
	gotoxy(105, 9); cout<<"SO NGAY QUA HAN";
	BoxNoiDung(str, dem, 5, 11, 118, 35);
}
void InserttionSort(int arr1[], int arr2[], int soluong){
	int i, j;
	int X, Y;
	for(int i = 1; i < soluong; i++){
		X = arr1[i];
		Y = arr2[i];
		for(j=i-1; j>=0 && X>arr1[j]; j--){
			arr1[j+1] = arr1[j];
			arr2[j+1] = arr2[j];
		}
		arr1[j+1] = X;
		arr2[j+1] = Y;
	}
}
void DociaMuonSachQuaHan(){
	int madocgiamuonqh[soluongdocgia];
	int songayqh[soluongdocgia];
	int soluongdocgiaquahan = 0;
	int songayqhan = 0;
	Queue<NODE_DOCGIA*> q;
	if(root != NULL)  q.Insert_queue(root);
	NODE_DOCGIA *nodeDG;
	bool hasDGQH;
	while(!q.empty()){
		nodeDG = q.GetFront();  q.Delete_queue();
		hasDGQH = false;
		int demchuatra = 0;
		if(nodeDG->docgia.dsmuontra.chuatra>0 && nodeDG->docgia.dsmuontra.First!=NULL){
			for(NODE_MUONTRA *nodeMT = nodeDG->docgia.dsmuontra.First; nodeMT != NULL; nodeMT = nodeMT->next){
				if(strlen(nodeMT->data.NgayTra) == 0){
					//chua tra sach
					if(DiffTime(GetSystemDate(), nodeMT->data.NgayMuon) > 7*24*60*60){
						songayqhan = DiffTime(GetSystemDate(), nodeMT->data.NgayMuon) / (24*60*60);
						hasDGQH = true;
					}
					if(++demchuatra >= nodeDG->docgia.dsmuontra.chuatra)  break; //toi da 3
				}
			}
		}
		if(hasDGQH){
			madocgiamuonqh[soluongdocgiaquahan] = nodeDG->docgia.MATHE;
			songayqh[soluongdocgiaquahan] = songayqhan;
			soluongdocgiaquahan++;
		}
		
		if(nodeDG->left != NULL)  q.Insert_queue(nodeDG->left);
		if(nodeDG->right != NULL)  q.Insert_queue(nodeDG->right);
	}
	if(soluongdocgiaquahan == 0){
		MessageBox(GetConsoleWindow(), "Khong co doc gia muon sach qua han!", "Thong bao!", MB_OK | MB_ICONINFORMATION);
		return;
	}
	InserttionSort(madocgiamuonqh, songayqh, soluongdocgiaquahan);
	DrawMuonSachQuaHan(songayqh, madocgiamuonqh, soluongdocgiaquahan);
}
int main(){
	SetConsoleTitle("QUAN LY THU VIEN");
	resizeConsole(1024, 720);
	SetScreenBufferSize(W, H);
	clrscr();
	Khung();
	ShowCur(false);
	TieuDe(30, 3);
	getch();
	XoaNoiDung(3, 38, 0);
	//TaoFileMaDocGia(); 
	Initialize(root);
	ReadMaDocGiaFromFile(madocgia);
	ReadDocGiaFromFile(root);
	ReadDauSachFromFile(DSDS);
	int chon;
	while(true){
		chon = MenuDong(ThucDon, 38, 7);
		switch(chon){
			case 1: {
				while(true){
					int chon1 = MenuCon(NhapDocGia, 3);
					if(chon1 == 1){
						DOCGIA dg;
						if(ThemDocGia(dg)){
							MenuChinhSuaDocGia(dg);
							Insert_DocGia(root, dg);
							soluongdocgia++;
							WriteDocGiaToFile(root);
						}
						else{
							vitridocgia--;
							swap(madocgia[vitridocgia], madocgia[rand()%MAX_MADOCGIA+(vitridocgia+1)]);
							WriteMaDocGiaToFile(madocgia);
							XoaNoiDung(7, 32, 0);
						}
					}
					else if(chon1 == 2){
						int madocgiaxoa = XoaDocGia();
						if(madocgiaxoa != -1){
							int vitri = 0;
							while(madocgia[vitri] != madocgiaxoa)  vitri++;
							swap(madocgia[vitri], madocgia[--vitridocgia]);
							WriteMaDocGiaToFile(madocgia);
							ReadMaDocGiaFromFile(madocgia);
							soluongdocgia--;
						}
						XoaNoiDung(1, 39, 0);
					}
					else if(chon1 == 3){
						HieuChinhThongTinDG();
						WriteDocGiaToFile(root);
						XoaNoiDung(1, 39, 0);
					}	
					else {
						//ColorBox(ToaDo(3, 1), ToaDo(120, 39), "", 0, 15);
						break;
					}
				}
			}
			break;
			case 2: {
				while(true){
					int chon2 = MenuCon(HienThiDocGia, 3);
					if(chon2 == 1){
						HoTenDocGiaTangDan();
						DrawDSDocGia();
					}
					else if(chon2 == 2){
						MaDocGiaTangDan();
						DrawDSDocGia();
					}
					else if(chon2 == 3){
						DociaMuonSachQuaHan();
					}
					else  break;
				}
			}break;
			case 3: {
				while(true){
					int chon3 = MenuCon(XuLyDauSach, 3);
					if(chon3 == 1){
						DAUSACH *dausach = new DAUSACH;
						if(ThemDauSach(dausach)){
							MenuChinhSuaDauSach(dausach);
							AddDauSach(DSDS, dausach);
							WriteDauSachToFile(DSDS);
						}
						else  XoaNoiDung(7, 32, 0);
					}
					else if(chon3==2){
						XoaDauSach();
						XoaNoiDung(7, 32, 0);
						WriteDauSachToFile(DSDS);
					}
					else if(chon3 == 3){
						HieuChinhThongTinDauSach();
						XoaNoiDung(7, 32, 0);
						WriteDauSachToFile(DSDS);
					}
					else{
						XoaNoiDung(7, 32, 0);
						break;
					}
				}
			}break;
			case 4: {
				while(true){
					int chon4 = MenuCon(DSDauSach, 2);
					if(chon4 == 1)       DrawDSDauSach();
					else if(chon4 == 2)  DrawTop10MuonTra();
					else{
						XoaNoiDung(1, 38, 0);
						break;
					}
				}	
			}break;
			case 5: {
				while(true){
					int chon5 = MenuCon(MuonSach, 2);
					if(chon5 == 1){
						XoaNoiDung(1, 4, 0);
						DrawMuonTra();
						MenuMuonSach();
						WriteDocGiaToFile(root);
						WriteDauSachToFile(DSDS);
					    XoaNoiDung(1, 38, 0);
					}
					else if(chon5 == 2){
						XoaNoiDung(1, 4, 0);
						DrawMuonTra();
						MenuTraSach();
						WriteDocGiaToFile(root);
						WriteDauSachToFile(DSDS);
						XoaNoiDung(1, 38, 0);
					}
					else break;
				}
			}break;
			case 6:   return 0;
		}
	}
	DeleteMemoryDocGia(root);
	DeleteMemoryDMS(DSDS);
	return 0;
}
