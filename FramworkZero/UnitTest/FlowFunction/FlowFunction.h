#pragma once

#include "stdafx.h"

// 유사 행동트리
// 서비스+블랙보드+미래반환 추가하면 -> 함수형 행동스택

// 제어 종료 이벤트
typedef function<void(void)> FutureAction;
#include "FutureReturn.h"

#include "FlowTesk.h"

// 흐름 제어 기초
#include "FlowReader.h"


// - 흐름 제어 -

typedef function<bool()> Judgment;
typedef function<int()> FuncCase;

#include "FlowControll/FlowAction.h"
#include "FlowControll/FlowRoutine.h"
#include "FlowControll/FlowIf.h"
#include "FlowControll/FlowLoop.h"
#include "FlowControll/FlowSwitching.h"