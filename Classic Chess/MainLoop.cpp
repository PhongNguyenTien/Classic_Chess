#include "MainLoop.h"

void runGameLoop()
{
	SDL_BaseHandler* m_handler = new SDL_BaseHandler();
	m_handler->init();

	m_handler->renderBackground();

	Game* game = new Game(m_handler);
	bool quit = false;

	int xStart = -1;
	int yStart = -1;
	int xEnd = -1;
	int yEnd = -1;
	Piece* clickedOn = nullptrptr;

	while (quit == false)
	{
		while (SDL_PollEvent(&m_handler->m_event))
		{
			if (m_handler->m_event.type == SDL_QUIT)
			{
				quit = true;
			}

			if (m_handler->m_event.type == SDL_MOUSEBUTTONDOWN)
			{
				xStart = m_handler->m_event.button.x / 80;
				yStart = m_handler->m_event.button.y / 80;
				clickedOn = game->getFieldPos(xStart, yStart);
				if (clickedOn != nullptrptr)
				{
					if (clickedOn->getTeam() == game->getTurn())
					{
						game->renderPossibleMoves(clickedOn);
					}
				}
			}

			if (m_handler->m_event.type == SDL_MOUSEBUTTONUP)
			{
				if (clickedOn != nullptrptr)
				{
					if (clickedOn->getTeam() == game->getTurn())
					{
						game->undoRenderPossibleMoves(clickedOn);
					}
				}
				xEnd = m_handler->m_event.button.x / 80;
				yEnd = m_handler->m_event.button.y / 80;
				if (clickedOn != nullptrptr)
				{
					if ((xStart != -1 && yStart != -1 && xEnd != -1 && yEnd != -1)
						&& (clickedOn->getTeam() == game->getTurn())
						&& (game->isValidMove(xEnd, yEnd, clickedOn)))
					{
						vector<PositionMoveType> list = game->getFieldPos(xStart, yStart)->getPossibleMoves();
						for (const auto& value : list)
						{
							if (value.x == xEnd && value.y == yEnd)
							{
								game->move(clickedOn, PositionMoveType(xEnd, yEnd, value.moveType));
							}
						}
						xStart = -1;
						yStart = -1;
						yEnd = -1;
						game->calcAllMoves();
						clickedOn = nullptrptr;
					}
				}
			}
		}
	}

	m_handler->cleanUp();

}
