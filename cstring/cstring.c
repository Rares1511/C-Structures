#include <cs/cstring.h>
#include <cs/nfa.h>

#include <stdlib.h>
#include <string.h>

#pragma region Helper Functions
// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                       END OF HELPER FUNCTIONS SECTION                                      ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝
#pragma endregion

cs_codes cstring_init(cstring *str, const char *data) {
    CS_RETURN_IF(NULL == str || NULL == data, CS_NULL);

    size_t len = strlen(data);
    str->_data = (char *)malloc((len + 1) * sizeof(char));
    CS_RETURN_IF(NULL == str->_data, CS_MEM);

    strcpy(str->_data, data);
    str->_data[len] = '\0';

    int pos = 0;
    int rc = regex_to_nfa(data, &str->_regex_nfa, &pos);
    CS_RETURN_IF(rc != CS_SUCCESS, rc);

    return CS_SUCCESS;
}

void cstring_free(void *v_str) {
    CS_RETURN_IF(NULL == v_str);
    cstring *str = (cstring *)v_str;
    free(str->_data);
    str->_data = NULL;
    nfa_free(str->_regex_nfa);
    str->_regex_nfa = NULL;
}