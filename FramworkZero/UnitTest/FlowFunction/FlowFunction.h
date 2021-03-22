#pragma once

#include "stdafx.h"

// ���� �ൿƮ��

// ���� ���� �̺�Ʈ
typedef function<void(void)> FutureAction;
#include "FutureReturn.h"

#include "FlowTesk.h"

// �帧 ���� ����
#include "FlowReader.h"


// - �帧 ���� -

typedef function<bool()> Judgment;

#include "FlowControll/FlowAction.h"
#include "FlowControll/FlowRoutine.h"
#include "FlowControll/FlowIf.h"
#include "FlowControll/FlowLoop.h"
#include "FlowControll/FlowSwitching.h"