
//
//	hsp3debug.cpp header
//
#ifndef __hsp3debug_h
#define __hsp3debug_h

// �G���[�R�[�h
typedef enum {

HSPERR_NONE = 0,				// �X�N���v�g�I����
HSPERR_UNKNOWN_CODE,
HSPERR_SYNTAX,
HSPERR_ILLEGAL_FUNCTION,
HSPERR_WRONG_EXPRESSION,
HSPERR_NO_DEFAULT,
HSPERR_TYPE_MISMATCH,
HSPERR_ARRAY_OVERFLOW,
HSPERR_LABEL_REQUIRED,
HSPERR_TOO_MANY_NEST,
HSPERR_RETURN_WITHOUT_GOSUB,
HSPERR_LOOP_WITHOUT_REPEAT,
HSPERR_FILE_IO,
HSPERR_PICTURE_MISSING,
HSPERR_EXTERNAL_EXECUTE,
HSPERR_PRIORITY,
HSPERR_TOO_MANY_PARAMETERS,
HSPERR_TEMP_BUFFER_OVERFLOW,
HSPERR_WRONG_NAME,
HSPERR_DIVIDED_BY_ZERO,
HSPERR_BUFFER_OVERFLOW,
HSPERR_UNSUPPORTED_FUNCTION,
HSPERR_EXPRESSION_COMPLEX,
HSPERR_VARIABLE_REQUIRED,
HSPERR_INTEGER_REQUIRED,
HSPERR_BAD_ARRAY_EXPRESSION,
HSPERR_OUT_OF_MEMORY,
HSPERR_TYPE_INITALIZATION_FAILED,
HSPERR_NO_FUNCTION_PARAMETERS,
HSPERR_STACK_OVERFLOW,
HSPERR_INVALID_PARAMETER,
HSPERR_INVALID_ARRAYSTORE,
HSPERR_INVALID_FUNCPARAM,
HSPERR_WINDOW_OBJECT_FULL,
HSPERR_INVALID_ARRAY,
HSPERR_STRUCT_REQUIRED,
HSPERR_INVALID_STRUCT_SOURCE,
HSPERR_INVALID_TYPE,
HSPERR_DLL_ERROR,
HSPERR_COMDLL_ERROR,
HSPERR_NORETVAL,
HSPERR_FUNCTION_SYNTAX,

HSPERR_INTJUMP,					// ���荞�݃W�����v��
HSPERR_EXITRUN,					// �O���t�@�C�����s
HSPERR_MAX

} HSPERROR;

char *hspd_geterror( HSPERROR error );


// Debug Info ID
enum
{
DEBUGINFO_GENERAL = 0,
DEBUGINFO_VARNAME,
DEBUGINFO_INTINFO,
DEBUGINFO_GRINFO,
DEBUGINFO_MMINFO,
DEBUGINFO_MAX
};

// Debug Flag ID
enum
{
HSPDEBUG_NONE = 0,
HSPDEBUG_RUN,
HSPDEBUG_STOP,
HSPDEBUG_STEPIN,
HSPDEBUG_STEPOVER,
HSPDEBUG_MAX
};

typedef struct HSP3DEBUG
{
	//	[in/out] tranfer value
	//	(�V�X�e���Ƃ̒ʐM�p)
	//
	int	flag;				// Flag ID
	int	line;				// �s�ԍ����
	char *fname;			// �t�@�C�������
	void *dbgwin;			// Debug Window��Handle
	char *dbgval;			// debug���擾�o�b�t�@

	//	[in] system value
	//	(��������ɐݒ肳��܂�)
	//
	//void 	*hspctx;
	struct HSPCTX 	*hspctx;
	//
	char *	(* get_value) (int);			// debug���擾�R�[���o�b�N
	char *	(* get_varinf) (char *,int);	// �ϐ����擾�R�[���o�b�N
	void	(* dbg_close) (char *);			// debug���擾�I��
	void	(* dbg_curinf)( void );			// ���ݍs�E�t�@�C�����̎擾
	int		(* dbg_set) (int);				// debug���[�h�ݒ�

} HSP3DEBUG;

#endif