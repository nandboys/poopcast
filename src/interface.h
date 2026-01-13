#pragma once

#include <net/if.h>
#include <stdio.h>
#include <stdlib.h>

unsigned int prompt_interface_index(struct if_nameindex *interfaces);

