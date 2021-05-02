
#define IP_HDR_SIZE_MIN 20
#define IP_ADDR_STR_LEN sizeof("255.255.255.255")

typedef uint32_t ip_addr_t;

int ip_init(void);
void ip_dump(const uint8_t *data, size_t len);
char *ip_addr_ntop(ip_addr_t src, char *addr, size_t len);