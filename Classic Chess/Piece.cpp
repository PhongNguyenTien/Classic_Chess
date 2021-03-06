#include "Piece.h"
#include "King.h"
#include <string>
#include <iostream>

vector<PositionMoveType> Piece::pushMove(vector<PositionMoveType> moveList,
	PositionMoveType move,
	King* king,
	Piece* field[8][8],
	bool checkCheck)
{
	if (!checkCheck)
	{
		moveList.push_back(move);
	}
	else
	{
		bool enemyPlace = true;
		king->setCheck(field, king->getPos().x, king->getPos().y);
		Piece* zwisch = field[move.x][move.y];
		enemyPlace = false;

		if (field[move.x][move.y] != nullptr)
		{
			enemyPlace = true;
			field[move.x][move.y] = nullptr;
		}

		swap(field[move.x][move.y], field[m_pos.x][m_pos.y]);
		if (m_type == KING)
		{
			king->setCheck(field, move.x, move.y);
		}
		else
		{
			king->setCheck(field, king->getPos().x, king->getPos().y);
		}
		swap(field[move.x][move.y], field[m_pos.x][m_pos.y]);

		if (enemyPlace)
		{
			field[move.x][move.y] = zwisch;
		}
		if (!king->getCheck())
		{
			moveList.push_back(move);
		}
		king->setCheck(field, king->getPos().x, king->getPos().y);
	}
	return moveList;
}

King* Piece::getOwnKing(Piece* field[8][8])
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (field[i][j] != nullptr)
			{
				if (field[i][j]->getTeam() == m_team && field[i][j]->getType() == KING)
				{
					return static_cast<King*>(field[i][j]);
				}
			}
		}
	}
	return nullptr;
}

Piece::Piece(Team team, Position pos, SDL_BaseHandler* m_handler, PieceType type)
	:m_team(team), m_pos(pos), m_handler(m_handler), m_texture(nullptr), m_hasMoved(false), m_type(type)
{
}

Piece::Piece(const Piece& piece)
	: m_team(piece.m_team), m_pos(piece.m_pos), m_handler(piece.m_handler), m_texture(nullptr), m_hasMoved(false), m_type(piece.m_type)
{
}

Piece::~Piece()
{
	SDL_DestroyTexture(m_texture);

	m_handler->undoPieceRender(m_pos.x, m_pos.y);
}


void Piece::render()
{
	SDL_Rect src = { 0, 0, 60, 60 };

	SDL_Rect dest = { m_handler->SCREEN_WIDTH / 8 * m_pos.x,
					  m_handler->SCREEN_HEIGHT / 8 * m_pos.y,
					  m_handler->SCREEN_WIDTH / 8,
					  m_handler->SCREEN_HEIGHT / 8 };
	m_handler->DrawRectangle(src, dest, m_texture);
}
