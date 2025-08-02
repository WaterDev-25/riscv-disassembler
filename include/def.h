#ifndef DEF_H_
#define DEF_H_

/**
 * Status codes for function returns
 */
typedef enum {
    STATUS_FAILURE = 0,
    STATUS_SUCCESS = 1
} STATUS;

/**
 * Check if operation was successful
 */
#define IS_SUCCESS(status) ((status) == STATUS_SUCCESS)

/**
 * Mark parameter as deliberately unused
 */
#define UNUSED(param) ((void)(param))

#endif /* !DEF_H_ */
