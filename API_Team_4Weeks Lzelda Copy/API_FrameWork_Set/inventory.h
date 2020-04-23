#pragma once

enum SHOPITEM;

enum ITEM;


struct tagInvenItem
{
	image* itemImage;
	SHOPITEM itemKind;

	RECT itemRc;

	bool isItem;		//아이템이 있는가?
};

class inventory
{
private:
	vector<tagInvenItem> _vInItem;
	vector<tagInvenItem>::iterator _viInItem;

	RECT _invenRc[4][5];	//인벤칸




public:
	inventory();
	~inventory();

	void init(RECT inven);
	void update();
	void release();
	void render(HDC hdc);

	void addItem(SHOPITEM itemKind);
	void addItem(tagInvenItem item);

	vector<tagInvenItem> getInItem() { return _vInItem; };
	void setInItem(vector<tagInvenItem>invenVector) { _vInItem = invenVector; };

	RECT getInvenRc(int x, int y) { return _invenRc[y][x]; };
	void getInvenRc(RECT inven);

	bool getIsItem(SHOPITEM item) { return _vInItem[(int)item].isItem; };
	void setIsItem(SHOPITEM item, bool set) { _vInItem[(int)item].isItem = set; };


};

