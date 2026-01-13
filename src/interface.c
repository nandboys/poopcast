#include "interface.h"

unsigned int prompt_interface_index(struct if_nameindex *interfaces) {
    if (interfaces == nullptr) {
        perror("if_nameindex");
        return 1;
    }

    size_t count = 0;
    for (
        struct if_nameindex *interface = interfaces;
        !(interface->if_index == 0 && interface->if_name == nullptr);
        ++interface
    ) {
        ++count;
        printf("%u.\t%s\n", interface->if_index, interface->if_name);
    }

    int interface = 0;
    while (true) {
        puts("----------------");
        fputs("Select interface: ", stdout);

        if (scanf("%d", &interface) != 1) {
            puts("Invalid input. Please enter a valid index.");

            int c;
            while ((c = getchar()) != '\n' && c != EOF);

            interface = 0;
            continue;
        }

        if (!(1 <= interface && interface <= count)) {
            puts("Invalid input. Index out of range.");
            continue;
        }

        break;
    }

    return interface;
}

