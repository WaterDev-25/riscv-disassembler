#ifndef DEF_H_
    #define DEF_H_

    #define FN_FAILURE  0x00000000
    #define FN_SUCCESS  0x00000001
    #define FN_NULL_PTR 0x00000002

    #define PR_SUCCESS  0x00000000
    #define PR_FAILURE  0x00000001

    #define UNUSED(v)   (void)(v)

typedef int FNSTATUS;
typedef int PRSTATUS;

#endif /* !DEF_H_ */
