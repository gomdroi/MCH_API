#include "stdafx.h"
#include "unitManager.h"
#include "unitSet.h"
#include "tile.h"

unitManager::unitManager()
{
}


unitManager::~unitManager()
{
}

HRESULT unitManager::init()
{
	unitSet* units;
	vector<tile*> mapVector = TILEMANAGER->getTileVector();
	//맵 타일 벡터를 돌면서 세팅 유닛정보를 수집해서 그자리에 실제 유닛을 배치해준다.
	for (int i = 0; i < mapVector.size(); i++)
	{
		if (mapVector[i]->getSettingUnitType() != TYPE_NONE)
		{
			
			units = new unitSet;
			units->init();
			units->setUnit(mapVector[i]->getSettingUnitType());
			units->setPosition(mapVector[i]->getPosition());
			mapVector[i]->setOnTileUnit(units);
			if (units->getUnitDef() == DEF_ALLY)
			{
				allyUnitVector.push_back(units);
			}
			else if (units->getUnitDef() == DEF_ENEMY)
			{
				enemyUnitVector.push_back(units);
			}
		}
	}
	return S_OK;
}

void unitManager::init(STAGE_NUMBER _stagenumber)
{
	
	
	/*units = new unitSet;
	units->init();
	units->setUnit(TYPE_ELIWOOD);
	units->setPosition(TILEMANAGER->getTileVector()[32]->getPosition());
	TILEMANAGER->getTileVector()[32]->setOnTileUnit(units);
	allyUnitVector.push_back(units);*/

}

void unitManager::update()
{
	for (int i = 0; i < allyUnitVector.size(); i++)
	{
		allyUnitVector[i]->update();
	}

	if (isEnemyTurn)
	{
		if (!isTurning)
		{
			int j = 0;
			for (j; j < enemyUnitVector.size(); j++) //적 벡터를 돌아서
			{
				if (!enemyUnitVector[j]->getIsTurnEnd()) //걸리는 적 유닛의 턴이 끝나지 않았다면
				{
					if (enemyUnitVector[j]->getIsTakingTurn()) break; //턴진행중인지 확인하고 진행중이면 검색중지
				}
			}

			if (j == enemyUnitVector.size()) //다 돌았는데 적 턴이 진행중인 넘이 없다
			{
				for (int i = 0; i < enemyUnitVector.size(); i++) //다시 적 벡터를 돌아서
				{
					if (!enemyUnitVector[i]->getIsTurnEnd()) //턴이 끝나지 않은 유닛을 찾아서
					{
						enemyUnitVector[i]->setIsTakingTurn(true); //턴진행중을 켜준다.
						g_cursor.setSelectedUnit(enemyUnitVector[i]);
						ASTAR->setMovingUnit(enemyUnitVector[i]);
						ASTAR->setStartTile(enemyUnitVector[i]->getOccupiedTile());
						break;
					}
				}
			}
		}	
	}

	for (int i = 0; i < enemyUnitVector.size(); i++)
	{
		enemyUnitVector[i]->update();
		//enemyUnitVector[i]->setTakingTurn(true);
	}

	
}

void unitManager::animation()
{
}

void unitManager::render()
{	
	/*for (int i = 0; i < enemyUnitVector.size(); i++)
	{
		enemyUnitVector[i]->render();
	}
	for (int i = 0; i < allyUnitVector.size(); i++)
	{
		allyUnitVector[i]->render();
	}*/


	//제트오더 렌더링벡터에 유닛을 모으고
	for (int i = 0; i < enemyUnitVector.size(); i++)
	{
		zOrderVector.push_back(enemyUnitVector[i]);
	}
	for (int i = 0; i < allyUnitVector.size(); i++)
	{
		zOrderVector.push_back(allyUnitVector[i]);
	}

	//정렬
	sort(zOrderVector.begin(), zOrderVector.end(), unitSet::cmp);

	//실제 렌더링
	for (int i = 0; i < zOrderVector.size(); i++)
	{
		zOrderVector[i]->render();
	}
	for (int i = 0; i < zOrderVector.size(); i++)
	{
		zOrderVector[i] = nullptr;
	}
	zOrderVector.clear();
	zOrderVector.shrink_to_fit();
}

void unitManager::release()
{
	vector<unitSet*>::iterator uIter = allyUnitVector.begin();
	vector<unitSet*>::iterator euIter = enemyUnitVector.begin();

	for (uIter; uIter != allyUnitVector.end();)
	{
		SAFE_DELETE((*uIter));
		uIter = allyUnitVector.erase(uIter);
	}

	for (euIter; euIter != enemyUnitVector.end();)
	{
		SAFE_DELETE((*euIter));
		euIter = enemyUnitVector.erase(euIter);
	}
}

void unitManager::deadCheck()
{
	vector<unitSet*>::iterator euIter = enemyUnitVector.begin();
	vector<unitSet*>::iterator uIter = allyUnitVector.begin();

	for (euIter; euIter != enemyUnitVector.end();)
	{
		if ((*euIter)->getIsDead())
		{
			SAFE_DELETE((*euIter));
			euIter = enemyUnitVector.erase(euIter);
		}
		else euIter++;
	}

	for (uIter; uIter != allyUnitVector.end();)
	{
		if ((*uIter)->getIsDead())
		{
			SAFE_DELETE((*uIter));
			uIter = allyUnitVector.erase(uIter);
		}
		else uIter++;
	}
}

void unitManager::endGameCheck()
{
	if (allyUnitVector.empty() && isEnemyTurn)
	{
		SCENEMANAGER->changeScene("메인메뉴");
	}
	else if (enemyUnitVector.empty() && isPlayerTurn)
	{
		switch (g_cursor.getStageNum())
		{
		case 1:
			g_cursor.setStageNum(2);
			break;
		case 2:
			g_cursor.setStageNum(3);
			break;
		case 3:
			g_cursor.setStageNum(4);
			break;
		default:
			break;
		}
		isEnemyTurn = false;
		isPlayerTurn = false;
		isTurning = false;
		SCENEMANAGER->changeScene("스테이지토크");
	}
	
}
