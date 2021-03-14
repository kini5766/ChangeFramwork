#pragma once

/*
함수 실행 후 바로 리턴이 되는 것이 아닌
마지막 업데이트를 마친 후 리턴
Cancel하게 되면 리턴하지 않음
*/
class IDelayFunction
{
public:
	// DelayReturn을 저장
	virtual void Call(const ReturnAction* result) = 0;
	// 함수 갱신
	virtual void Update() = 0;
	// DelayReturn를 삭제(DelayReturn를 호출 안 함)
	virtual void Cancel() = 0;
};