#pragma once

/*
함수 실행 후 바로 리턴이 되는 것이 아닌
마지막 업데이트를 마친 후 리턴
Cancel하게 되면 리턴하지 않음
*/
class FlowTesk : public FlowBase
{
public:
	FlowTesk() { classType = FlowBase::ClassType::FlowTesk; }

	// DelayReturn을 저장
	function<void(const FutureAction* future)> FuncCall;

	// 함수 갱신
	function<void()> FuncUpdate;

	// DelayReturn를 삭제(DelayReturn를 호출 안 함)
	function<void()> FuncCancel;
};
