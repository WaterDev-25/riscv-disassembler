#ifndef DEF_H_
    #define DEF_H_

    #define FN_FAILURE  0x00000000
    #define FN_SUCCESS  0x00000001

    #define SUCCESS(v) (v == FN_SUCCESS)

    #define UNUSED(v)   (void)(v)

typedef int FNSTATUS;

#endif /* !DEF_H_ */
