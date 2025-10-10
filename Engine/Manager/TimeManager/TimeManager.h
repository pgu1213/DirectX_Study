#pragma once
#include <Engine/Manager/SingletonManager/Singleton.h>

class TimeManager : public Singleton<TimeManager>
{
    friend class Singleton<TimeManager>;
private:
    explicit TimeManager();
public:
    virtual ~TimeManager();
public:
    static bool Init(); // TimeManager �ʱ�ȭ �Լ�
    static void Update(); // �� ������ ȣ���Ͽ� ��Ÿ �ð��� ������Ʈ�ϴ� �Լ�
    static float GetDeltaTime(); // ���� �������� ��Ÿ �ð�(��)�� ��ȯ�ϴ� �Լ�
    static float GetTotalTime(); // ���α׷� ���� �� �� ��� �ð�(��)�� ��ȯ�ϴ� �Լ�
    static double GetTotalElapsedTimeDouble(); // ���е��� ���ؼ� double�� ����ؾ� �� ��

private:
    static LARGE_INTEGER m_PreviousCounter; // ���� �������� ī����
    static LARGE_INTEGER m_Frequency; // ���� ī������ ���ļ�
    static double m_CurrentDeltaTime; // ���� �������� ��Ÿ �ð�
    static double m_TotalElapsedTime; // �� ��� �ð�
};

