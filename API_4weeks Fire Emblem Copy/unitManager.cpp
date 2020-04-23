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
	//�� Ÿ�� ���͸� ���鼭 ���� ���������� �����ؼ� ���ڸ��� ���� ������ ��ġ���ش�.
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
			for (j; j < enemyUnitVector.size(); j++) //�� ���͸� ���Ƽ�
			{
				if (!enemyUnitVector[j]->getIsTurnEnd()) //�ɸ��� �� ������ ���� ������ �ʾҴٸ�
				{
					if (enemyUnitVector[j]->getIsTakingTurn()) break; //������������ Ȯ���ϰ� �������̸� �˻�����
				}
			}

			if (j == enemyUnitVector.size()) //�� ���Ҵµ� �� ���� �������� ���� ����
			{
				for (int i = 0; i < enemyUnitVector.size(); i++) //�ٽ� �� ���͸� ���Ƽ�
				{
					if (!enemyUnitVector[i]->getIsTurnEnd()) //���� ������ ���� ������ ã�Ƽ�
					{
						enemyUnitVector[i]->setIsTakingTurn(true); //���������� ���ش�.
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


	//��Ʈ���� ���������Ϳ� ������ ������
	for (int i = 0; i < enemyUnitVector.size(); i++)
	{
		zOrderVector.push_back(enemyUnitVector[i]);
	}
	for (int i = 0; i < allyUnitVector.size(); i++)
	{
		zOrderVector.push_back(allyUnitVector[i]);
	}

	//����
	sort(zOrderVector.begin(), zOrderVector.end(), unitSet::cmp);

	//���� ������
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
		SCENEMANAGER->changeScene("���θ޴�");
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
		SCENEMANAGER->changeScene("����������ũ");
	}
	
}
