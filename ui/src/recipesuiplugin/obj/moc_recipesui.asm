; Listing generated by Microsoft (R) Optimizing Compiler Version 19.00.24215.1 

include listing.inc

INCLUDELIB MSVCRTD
INCLUDELIB OLDNAMES

CONST	SEGMENT
$SG100465 DB	'C:\Qt\Qt5.9.3\5.9.3\msvc2015_64\include\QtCore/qmetatype'
	DB	'.h', 00H
	ORG $+5
$SG100466 DB	'qRegisterNormalizedMetaType was called with a not normal'
	DB	'ized type name, please call qRegisterMetaType instead.', 00H
	ORG $+1
$SG100467 DB	'qRegisterNormalizedMetaType', 00H
	ORG $+4
?qt_meta_stringdata_RecipesUI@@3Uqt_meta_stringdata_RecipesUI_t@@B DD 0ffffffffH ; qt_meta_stringdata_RecipesUI
	DD	09H
	DD	00H
	ORG $+4
	DQ	0000000000000018H
	DB	'RecipesUI', 00H
	ORG $+6
?qt_meta_data_RecipesUI@@3QBIB DD 07H			; qt_meta_data_RecipesUI
	DD	00H
	DD	00H
	DD	00H
	DD	00H
	DD	00H
	DD	00H
	DD	00H
	DD	00H
	DD	00H
	DD	00H
	DD	00H
	DD	00H
	DD	00H
	DD	00H
	ORG $+4
$SG100767 DB	'C:\Qt\Qt5.9.3\5.9.3\msvc2015_64\include\QtCore/qmetatype'
	DB	'.h', 00H
	ORG $+13
$SG100768 DB	'qRegisterNormalizedMetaType was called with a not normal'
	DB	'ized type name, please call qRegisterMetaType instead.', 00H
	ORG $+1
$SG100769 DB	'qRegisterNormalizedMetaType', 00H
	ORG $+4
$SG101069 DB	'C:\Qt\Qt5.9.3\5.9.3\msvc2015_64\include\QtCore/qmetatype'
	DB	'.h', 00H
	ORG $+5
$SG101070 DB	'qRegisterNormalizedMetaType was called with a not normal'
	DB	'ized type name, please call qRegisterMetaType instead.', 00H
	ORG $+1
$SG101071 DB	'qRegisterNormalizedMetaType', 00H
CONST	ENDS
PUBLIC	?__empty_global_delete@@YAXPEAX@Z		; __empty_global_delete
PUBLIC	?__empty_global_delete@@YAXPEAX_K@Z		; __empty_global_delete
PUBLIC	??C?$QScopedPointer@VQObjectData@@U?$QScopedPointerDeleter@VQObjectData@@@@@@QEBAPEAVQObjectData@@XZ ; QScopedPointer<QObjectData,QScopedPointerDeleter<QObjectData> >::operator->
PUBLIC	?metaObject@RecipesUI@@UEBAPEBUQMetaObject@@XZ	; RecipesUI::metaObject
PUBLIC	?qt_metacast@RecipesUI@@UEAAPEAXPEBD@Z		; RecipesUI::qt_metacast
PUBLIC	?qt_metacall@RecipesUI@@UEAAHW4Call@QMetaObject@@HPEAPEAX@Z ; RecipesUI::qt_metacall
PUBLIC	?qt_static_metacall@RecipesUI@@CAXPEAVQObject@@W4Call@QMetaObject@@HPEAPEAX@Z ; RecipesUI::qt_static_metacall
PUBLIC	?staticMetaObject@RecipesUI@@2UQMetaObject@@B	; RecipesUI::staticMetaObject
EXTRN	strcmp:PROC
EXTRN	__imp_?dynamicMetaObject@QObjectData@@QEBAPEAUQMetaObject@@XZ:PROC
EXTRN	__imp_?qt_metacast@QDialog@@UEAAPEAXPEBD@Z:PROC
EXTRN	__imp_?qt_metacall@QDialog@@UEAAHW4Call@QMetaObject@@HPEAPEAX@Z:PROC
EXTRN	__imp_?staticMetaObject@QDialog@@2UQMetaObject@@B:BYTE
_BSS	SEGMENT
?staticMetaObject@RecipesUI@@2UQMetaObject@@B DB 030H DUP (?) ; RecipesUI::staticMetaObject
_BSS	ENDS
pdata	SEGMENT
$pdata$?metaObject@RecipesUI@@UEBAPEBUQMetaObject@@XZ DD imagerel $LN5
	DD	imagerel $LN5+88
	DD	imagerel $unwind$?metaObject@RecipesUI@@UEBAPEBUQMetaObject@@XZ
$pdata$?qt_metacast@RecipesUI@@UEAAPEAXPEBD@Z DD imagerel $LN5
	DD	imagerel $LN5+75
	DD	imagerel $unwind$?qt_metacast@RecipesUI@@UEAAPEAXPEBD@Z
$pdata$?qt_metacall@RecipesUI@@UEAAHW4Call@QMetaObject@@HPEAPEAX@Z DD imagerel $LN3
	DD	imagerel $LN3+61
	DD	imagerel $unwind$?qt_metacall@RecipesUI@@UEAAHW4Call@QMetaObject@@HPEAPEAX@Z
pdata	ENDS
CRT$XCU	SEGMENT
??staticMetaObject$initializer$@RecipesUI@@2P6AXXZEA@@3P6AXXZEA DQ FLAT:??__E?staticMetaObject@RecipesUI@@2UQMetaObject@@B@@YAXXZ ; ??staticMetaObject$initializer$@RecipesUI@@2P6AXXZEA@@3P6AXXZEA
CRT$XCU	ENDS
xdata	SEGMENT
$unwind$?metaObject@RecipesUI@@UEBAPEBUQMetaObject@@XZ DD 010901H
	DD	06209H
$unwind$?qt_metacast@RecipesUI@@UEAAPEAXPEBD@Z DD 010e01H
	DD	0420eH
$unwind$?qt_metacall@RecipesUI@@UEAAHW4Call@QMetaObject@@HPEAPEAX@Z DD 011701H
	DD	04217H
xdata	ENDS
; Function compile flags: /Odtp
;	COMDAT ??__E?staticMetaObject@RecipesUI@@2UQMetaObject@@B@@YAXXZ
text$di	SEGMENT
??__E?staticMetaObject@RecipesUI@@2UQMetaObject@@B@@YAXXZ PROC ; `dynamic initializer for 'RecipesUI::staticMetaObject'', COMDAT
; File e:\g12_work\weidali_code\b\g12_4_b\ui\src\recipesuiplugin\x64\debug\moc\moc_recipesui.cpp
; Line 66
	mov	rax, QWORD PTR __imp_?staticMetaObject@QDialog@@2UQMetaObject@@B
	mov	QWORD PTR ?staticMetaObject@RecipesUI@@2UQMetaObject@@B, rax
	lea	rax, OFFSET FLAT:?qt_meta_stringdata_RecipesUI@@3Uqt_meta_stringdata_RecipesUI_t@@B
	mov	QWORD PTR ?staticMetaObject@RecipesUI@@2UQMetaObject@@B+8, rax
; Line 67
	lea	rax, OFFSET FLAT:?qt_meta_data_RecipesUI@@3QBIB
	mov	QWORD PTR ?staticMetaObject@RecipesUI@@2UQMetaObject@@B+16, rax
	lea	rax, OFFSET FLAT:?qt_static_metacall@RecipesUI@@CAXPEAVQObject@@W4Call@QMetaObject@@HPEAPEAX@Z ; RecipesUI::qt_static_metacall
	mov	QWORD PTR ?staticMetaObject@RecipesUI@@2UQMetaObject@@B+24, rax
	mov	QWORD PTR ?staticMetaObject@RecipesUI@@2UQMetaObject@@B+32, 0
	mov	QWORD PTR ?staticMetaObject@RecipesUI@@2UQMetaObject@@B+40, 0
	ret	0
??__E?staticMetaObject@RecipesUI@@2UQMetaObject@@B@@YAXXZ ENDP ; `dynamic initializer for 'RecipesUI::staticMetaObject''
text$di	ENDS
; Function compile flags: /Odtp
_TEXT	SEGMENT
_o$ = 8
_c$ = 16
_id$ = 24
_a$ = 32
?qt_static_metacall@RecipesUI@@CAXPEAVQObject@@W4Call@QMetaObject@@HPEAPEAX@Z PROC ; RecipesUI::qt_static_metacall
; File e:\g12_work\weidali_code\b\g12_4_b\ui\src\recipesuiplugin\x64\debug\moc\moc_recipesui.cpp
; Line 58
	mov	QWORD PTR [rsp+32], r9
	mov	DWORD PTR [rsp+24], r8d
	mov	DWORD PTR [rsp+16], edx
	mov	QWORD PTR [rsp+8], rcx
; Line 63
	ret	0
?qt_static_metacall@RecipesUI@@CAXPEAVQObject@@W4Call@QMetaObject@@HPEAPEAX@Z ENDP ; RecipesUI::qt_static_metacall
_TEXT	ENDS
; Function compile flags: /Odtp
_TEXT	SEGMENT
this$ = 48
_c$ = 56
_id$ = 64
_a$ = 72
?qt_metacall@RecipesUI@@UEAAHW4Call@QMetaObject@@HPEAPEAX@Z PROC ; RecipesUI::qt_metacall
; File e:\g12_work\weidali_code\b\g12_4_b\ui\src\recipesuiplugin\x64\debug\moc\moc_recipesui.cpp
; Line 85
$LN3:
	mov	QWORD PTR [rsp+32], r9
	mov	DWORD PTR [rsp+24], r8d
	mov	DWORD PTR [rsp+16], edx
	mov	QWORD PTR [rsp+8], rcx
	sub	rsp, 40					; 00000028H
; Line 86
	mov	r9, QWORD PTR _a$[rsp]
	mov	r8d, DWORD PTR _id$[rsp]
	mov	edx, DWORD PTR _c$[rsp]
	mov	rcx, QWORD PTR this$[rsp]
	call	QWORD PTR __imp_?qt_metacall@QDialog@@UEAAHW4Call@QMetaObject@@HPEAPEAX@Z
	mov	DWORD PTR _id$[rsp], eax
; Line 87
	mov	eax, DWORD PTR _id$[rsp]
; Line 88
	add	rsp, 40					; 00000028H
	ret	0
?qt_metacall@RecipesUI@@UEAAHW4Call@QMetaObject@@HPEAPEAX@Z ENDP ; RecipesUI::qt_metacall
_TEXT	ENDS
; Function compile flags: /Odtp
_TEXT	SEGMENT
this$ = 48
_clname$ = 56
?qt_metacast@RecipesUI@@UEAAPEAXPEBD@Z PROC		; RecipesUI::qt_metacast
; File e:\g12_work\weidali_code\b\g12_4_b\ui\src\recipesuiplugin\x64\debug\moc\moc_recipesui.cpp
; Line 77
$LN5:
	mov	QWORD PTR [rsp+16], rdx
	mov	QWORD PTR [rsp+8], rcx
	sub	rsp, 40					; 00000028H
; Line 78
	cmp	QWORD PTR _clname$[rsp], 0
	jne	SHORT $LN2@qt_metacas
	xor	eax, eax
	jmp	SHORT $LN1@qt_metacas
$LN2@qt_metacas:
; Line 79
	lea	rdx, OFFSET FLAT:?qt_meta_stringdata_RecipesUI@@3Uqt_meta_stringdata_RecipesUI_t@@B+24
	mov	rcx, QWORD PTR _clname$[rsp]
	call	strcmp
	test	eax, eax
	jne	SHORT $LN3@qt_metacas
; Line 80
	mov	rax, QWORD PTR this$[rsp]
	jmp	SHORT $LN1@qt_metacas
$LN3@qt_metacas:
; Line 81
	mov	rdx, QWORD PTR _clname$[rsp]
	mov	rcx, QWORD PTR this$[rsp]
	call	QWORD PTR __imp_?qt_metacast@QDialog@@UEAAPEAXPEBD@Z
$LN1@qt_metacas:
; Line 82
	add	rsp, 40					; 00000028H
	ret	0
?qt_metacast@RecipesUI@@UEAAPEAXPEBD@Z ENDP		; RecipesUI::qt_metacast
_TEXT	ENDS
; Function compile flags: /Odtp
_TEXT	SEGMENT
tv82 = 32
this$ = 64
?metaObject@RecipesUI@@UEBAPEBUQMetaObject@@XZ PROC	; RecipesUI::metaObject
; File e:\g12_work\weidali_code\b\g12_4_b\ui\src\recipesuiplugin\x64\debug\moc\moc_recipesui.cpp
; Line 72
$LN5:
	mov	QWORD PTR [rsp+8], rcx
	sub	rsp, 56					; 00000038H
; Line 73
	mov	rax, QWORD PTR this$[rsp]
	add	rax, 8
	mov	rcx, rax
	call	??C?$QScopedPointer@VQObjectData@@U?$QScopedPointerDeleter@VQObjectData@@@@@@QEBAPEAVQObjectData@@XZ ; QScopedPointer<QObjectData,QScopedPointerDeleter<QObjectData> >::operator->
	cmp	QWORD PTR [rax+40], 0
	je	SHORT $LN3@metaObject
	mov	rax, QWORD PTR this$[rsp]
	add	rax, 8
	mov	rcx, rax
	call	??C?$QScopedPointer@VQObjectData@@U?$QScopedPointerDeleter@VQObjectData@@@@@@QEBAPEAVQObjectData@@XZ ; QScopedPointer<QObjectData,QScopedPointerDeleter<QObjectData> >::operator->
	mov	rcx, rax
	call	QWORD PTR __imp_?dynamicMetaObject@QObjectData@@QEBAPEAUQMetaObject@@XZ
	mov	QWORD PTR tv82[rsp], rax
	jmp	SHORT $LN4@metaObject
$LN3@metaObject:
	lea	rax, OFFSET FLAT:?staticMetaObject@RecipesUI@@2UQMetaObject@@B ; RecipesUI::staticMetaObject
	mov	QWORD PTR tv82[rsp], rax
$LN4@metaObject:
	mov	rax, QWORD PTR tv82[rsp]
; Line 74
	add	rsp, 56					; 00000038H
	ret	0
?metaObject@RecipesUI@@UEBAPEBUQMetaObject@@XZ ENDP	; RecipesUI::metaObject
_TEXT	ENDS
; Function compile flags: /Odtp
;	COMDAT ??C?$QScopedPointer@VQObjectData@@U?$QScopedPointerDeleter@VQObjectData@@@@@@QEBAPEAVQObjectData@@XZ
_TEXT	SEGMENT
this$ = 8
??C?$QScopedPointer@VQObjectData@@U?$QScopedPointerDeleter@VQObjectData@@@@@@QEBAPEAVQObjectData@@XZ PROC ; QScopedPointer<QObjectData,QScopedPointerDeleter<QObjectData> >::operator->, COMDAT
; File c:\qt\qt5.9.3\5.9.3\msvc2015_64\include\qtcore\qscopedpointer.h
; Line 117
	mov	QWORD PTR [rsp+8], rcx
; Line 118
	mov	rax, QWORD PTR this$[rsp]
	mov	rax, QWORD PTR [rax]
; Line 119
	ret	0
??C?$QScopedPointer@VQObjectData@@U?$QScopedPointerDeleter@VQObjectData@@@@@@QEBAPEAVQObjectData@@XZ ENDP ; QScopedPointer<QObjectData,QScopedPointerDeleter<QObjectData> >::operator->
_TEXT	ENDS
; Function compile flags: /Odtp
;	COMDAT ?__empty_global_delete@@YAXPEAX_K@Z
_TEXT	SEGMENT
__formal$ = 8
__formal$ = 16
?__empty_global_delete@@YAXPEAX_K@Z PROC		; __empty_global_delete, COMDAT
; File e:\g12_work\weidali_code\b\g12_4_b\ui\src\recipesuiplugin\x64\debug\moc\moc_recipesui.cpp
; Line 91
	mov	QWORD PTR [rsp+16], rdx
	mov	QWORD PTR [rsp+8], rcx
	ret	0
?__empty_global_delete@@YAXPEAX_K@Z ENDP		; __empty_global_delete
_TEXT	ENDS
; Function compile flags: /Odtp
;	COMDAT ?__empty_global_delete@@YAXPEAX@Z
_TEXT	SEGMENT
__formal$ = 8
?__empty_global_delete@@YAXPEAX@Z PROC			; __empty_global_delete, COMDAT
; File e:\g12_work\weidali_code\b\g12_4_b\ui\src\recipesuiplugin\x64\debug\moc\moc_recipesui.cpp
; Line 91
	mov	QWORD PTR [rsp+8], rcx
	ret	0
?__empty_global_delete@@YAXPEAX@Z ENDP			; __empty_global_delete
_TEXT	ENDS
END
