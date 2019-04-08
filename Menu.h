#pragma once

/*
 * Menu.h
 *
 *  Created on: 16 нояб. 2018 г.
 *      Author: Кисуля
 */

#ifndef MENU_H_
#define MENU_H_


struct IMenu {
public:
	virtual ~IMenu() = default;
	virtual void right() = 0;
	virtual void left() = 0;
	virtual void open(IMenu** pointer) = 0;
	virtual void show() = 0;
	virtual void showName() = 0;
	virtual void addItem(IMenu* item) = 0;
	virtual void setNext(IMenu* element) = 0;
	virtual void setPrev(IMenu* element) = 0;
	virtual void setParrent(IMenu* parrent) = 0;
	virtual void fromBegin() = 0;
	virtual IMenu* getNext() = 0;
	virtual IMenu* getPrev() = 0;
private:
};

struct MenuItem : IMenu {
public:
	MenuItem(const char* name, char * firstStr, char * secStr);
	void right();
	void left();
	void open(IMenu** pointer);
	void show();
	void showName();
	void addItem(IMenu* item) {};
	void setNext(IMenu* element);
	void setPrev(IMenu* element);
	void setParrent(IMenu* parrent);
	void fromBegin();
	IMenu* getNext();
	IMenu* getPrev();

protected:
	const char* _name;
	IMenu *_parrent;
	IMenu *_nextElement;
	IMenu *_prevElement;
	char * _firstStr;
	char * _secStr;
};


struct MenuItemBool : MenuItem{
public:
	MenuItemBool(const char * name, char * firstStr, char * secStr,bool * data ,const char * trueState = "true", const char * falseState = "false");
	void right();
	void left();
	void show();
	void showName();
private:
	bool * data;
	const char * _trueState;
	const char * _falseState;
	char * _firstStr;
	char * _secStr;
};

struct MenuItemInt : MenuItem{
public:
	MenuItemInt(const char* name, char * firstStr, char * secStr, int * data , int  dataMax, int dataMin, const char * pref = "");
	void right();
	void left();
	void show();
	void showName();
private:
	int * _data;
	int _dataMax;
	int _dataMin;
	const char * _pref;
};

struct MenuItemFloat : MenuItem {
public:
	MenuItemFloat(const char* name, char * firstStr, char * secStr, float * data, float  dataMax, float dataMin, float plusData, const char * pref = "");
	void right();
	void left();
	void show();
	void showName();
private:
	float * _data;
	float _plusData;
	float _dataMax;
	float _dataMin;
	const char * _pref;
};

struct MenuItemEvent : MenuItem {
public:
	MenuItemEvent(const char* name, char * firstStr, char * secStr, void(*event)(void), const char * pref = "do..");
	void open(IMenu** pointer);
	void show();
	void showName();
private:
	void(*_event)(void);
	const char * _pref;
};

struct Menu : IMenu {
public:
	~Menu() = default;
	Menu(const char* name, char * firstStr, char * secStr);
	void right();
	void left();
	void open(IMenu** pointer);
	void show();
	void showName();
	void addItem(IMenu* item);
	void setNext(IMenu* element);
	void setPrev(IMenu* element);
	void setParrent(IMenu* parrent);
	void fromBegin();
	IMenu* getNext();
	IMenu* getPrev();
private:
	MenuItem back;
	const char* _name;
	IMenu *_parrent;
	IMenu *_curItem;
	IMenu *_firstItem;
	IMenu *_lastItem;
	IMenu *_nextElement;
	IMenu *_prevElement;
	char * _firstStr;
	char * _secStr;
};

struct MainScreen : IMenu {
public:
	MainScreen(Menu * menu, char * firstStr, char * secStr);
	void right();
	void left();
	void open(IMenu** pointer);
	void show();
	void showName();
	void addItem(IMenu* item);
	void setNext(IMenu* element);
	void setPrev(IMenu* element);
	void setParrent(IMenu* parrent);
	void fromBegin();
	IMenu* getNext();
	IMenu* getPrev();
private:
	Menu * menu;
	int * distance;
	char * _firstStr;
	char * _secStr;
};


#endif /* MENU_H_ */
