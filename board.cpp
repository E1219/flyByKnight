#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include "board.hpp"
#include "game.hpp"
#include "piece.hpp"

using namespace std;

Board::Board(Game * g){
  gm = g;
  clearBoard();
}

void Board::clearBoard(){
  for(int i = 0; i < 8; i++)
    for(int j = 0; j < 8; j++){
      if(pieces[i][j] != NULL)
		delete pieces[i][j];
      pieces[i][j] = NULL;
    }
 }

void Board::placePiece(Piece * piece, int x, int y){
  removePiece(x, y);
  pieces[x][y] = piece;
  if(piece != NULL)
    piece->setLocation(x, y);
}
void Board::removePiece(int x, int y){
  if(pieces[x][y] != NULL)
    delete pieces[x][y];
}
Piece * Board::getPiece(int x, int y){
	return pieces[x][y];
}
void Board::printBoard(){
  /*cout << " |--|--|--|--|--|--|--|--|" << endl;
  for(int i = 7; i >= 0; i--){
    cout << (i + 1) << "|";
    for(int j = 0 ; j < 8; j++)
      cout << (pieces[j][i] == NULL? "  ":pieces[j][i]->toShortString()) << "|";
    cout << endl << " |--|--|--|--|--|--|--|--|" << endl;
  }
  
  cout  << "   A  B  C  D  E  F  G  H" << endl;*/
  
for(int i = 7; i >= 0; i--){
    cout << i + 1 << "  ";
    for(int j = 0 ; j < 8; j++)
      cout << (pieces[j][i] == NULL? (((i+j) % 2 == 0)? "#": ".") :pieces[j][i]->toShortString()) << " ";
    cout << endl;
  }
  cout << /*"   ---------------" <<*/ endl <<  "   a b c d e f g h" << endl;
}
void Board::clearSpace(int x, int y){
  pieces[x][y]  = NULL;
}

int Board::toInts(string str){

  if(str.length() < 2)
    return -1;

  int ret = 0;
  ret = str.at(0)-97;
  if(ret > 7 || ret < 0)
    return -1;
  if(str.at(1) > 56 || str.at(1) < 49)
    return -1;

  return ((ret * 10) + str.at(1) - 49);
}
string Board::toStr(int mov){
  
  if((mov / 10) >= 8 || (mov / 10) < 0 || (mov % 10) >= 8 || (mov % 10) < 0 )
     return "...---...";

  string str = "";
  str += (mov / 10) + 97;
  str += (mov % 10) + 49;
  return str;
}
bool Board::moveFormat(string str){
  
  return (Board::toInts(str) != -1 && Board::toInts(str.substr(2,2)) != -1 && (str.length() == 4 || (str.length() == 5 && (str.at(4) == 'b' || str.at(4) == 'n' || str.at(4) == 'r' || str.at(4) == 'q'))));
}

void Board::getPieces(int color, vector<Piece *> &found){
  for(int i = 0; i < 8; i++)
    for(int j = 0; j < 8; j++)
      if(pieces[i][j] != NULL && pieces[i][j]->getColor() == color)
	found.push_back(pieces[i][j]);
}

bool Board::promotePawn(char newPiece){
  
  int row = 0;
  for(int i = 0; i < 2; i++){
    for(int j = 0; j < 8; j++){
      Piece * p = pieces[j][row];
      if(p != NULL && (p->toShortString().at(0) == 'P' || p->toShortString().at(0) == 'p')){
	Piece * nPiece;
	switch(newPiece){
	case 'N':
	case 'n':
	  nPiece = new Knight(p->getColor(), p->getGame());
	  break;
	case 'B':
	case 'b':
	  nPiece = new Bishop(p->getColor(), p->getGame());
	  break;
	case 'R':
	case 'r':
	  nPiece = new Rook(p->getColor(), p->getGame());
	  static_cast<Rook *>(nPiece)->hasMoved = true;
	  break;
	case 'Q':
	case 'q':
	default:
	  nPiece = new Queen(p->getColor(), p->getGame());
	  break;
	}
	
	if(p != NULL){
	  change_t c;
	  c.moded = p;
	  c.newLoc = j*10 + row;
	  c.oldLoc = -11;
	  c.captured = false;
	  c.firstMove = false;
	  c.ep = false;

	  gm->addChange(c);
	}
	
	pieces[j][row] = nPiece;
	nPiece->setLocation(j,row);

	change_t c;
	c.moded = nPiece;
	c.oldLoc = -11;
	c.newLoc = j*10 + row;
	c.captured = false;
	c.firstMove = false;
	c.ep = false;
	
	gm->addChange(c);	
      }
    }
    row = 7;
  }
  
  return false;
}
