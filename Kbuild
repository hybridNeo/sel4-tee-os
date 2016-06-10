#
# Copyright 2015, NICTA
#
# This software may be distributed and modified according to the terms of
# the BSD 2-Clause license. Note that NO WARRANTY is provided.
# See "LICENSE_BSD2.txt" for details.
#
# @TAG(NICTA_BSD)
#

apps-$(CONFIG_APP_TEE_OS) += tee-os

# list of libraries the app needs to build
tee-os-y = common libsel4 libmuslc \
            libsel4muslcsys \
            libsel4simple libsel4vka libsel4allocman libsel4vspace \
	    libsel4platsupport \
            libsel4utils \
            libcpio libelf \
            libutils

# set up correct simple lib for our kernel
ifdef CONFIG_KERNEL_STABLE
tee-os-$(CONFIG_LIB_SEL4_SIMPLE_STABLE) += libsel4simple-stable
else
tee-os-$(CONFIG_LIB_SEL4_SIMPLE_DEFAULT) += libsel4simple-default
endif

# add the companion app as a component so that we can elf load it
tee-os-components-y += hello-4-app
tee-os-components-y += client-os
tee-os-components = $(addprefix $(STAGE_BASE)/bin/, $(tee-os-components-y))

tee-os: export COMPONENTS=${tee-os-components}
tee-os: ${tee-os-components-y} kernel_elf $(tee-os-y)
