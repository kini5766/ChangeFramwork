#pragma once

#include "stdafx.h"

// 유사 행동트리

// 제어 종료 이벤트
typedef function<void(void)> FutureAction;
#include "FutureReturn.h"

// 유사 행동 노드
#include "FlowTesk.h"

// 흐름 제어 베이스
#include "FlowReader.h"


// - 흐름 제어 -

// 유사 시퀀스
#include "FlowControll/FlowRoutine.h"