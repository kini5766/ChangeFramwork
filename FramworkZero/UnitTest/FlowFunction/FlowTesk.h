#pragma once

/*
함수 실행 후 바로 리턴이 되는 것이 아닌
마지막 업데이트를 마친 후 리턴
Hold하게 되면 리턴하지 않음
*/
class IFlowTesk
{
public:
	// 이 행동이 선택되어 시작함
	virtual void Call(const FutureAction* future) = 0;

	// 이 행동 갱신 매 프래임 호출
	virtual void Update() {};

	// 일시 중지되고 다른 행동이 실행될 경우 호출
	virtual void Hold() {};
};
