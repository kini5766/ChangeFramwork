#pragma once

#include "stdafx.h"

// ���� �ൿƮ��
// ����+������+�̷���ȯ �߰��ϸ� -> �Լ��� �ൿ����

// ���� ���� �̺�Ʈ
typedef function<void(void)> FutureAction;
#include "FutureReturn.h"

#include "FlowTesk.h"

// �帧 ���� ����
#include "FlowReader.h"


// - �帧 ���� -

typedef function<bool()> Judgment;
typedef function<int()> FuncCase;

#include "FlowControll/FlowAction.h"
#include "FlowControll/FlowRoutine.h"
#include "FlowControll/FlowIf.h"
#include "FlowControll/FlowLoop.h"
#include "FlowControll/FlowSwitching.h"