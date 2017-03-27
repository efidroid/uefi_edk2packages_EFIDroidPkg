#include <Library/LKEnvLib.h>
#include <Library/fbcon.h>
#include <Chipset/mdp5.h>
#include <Platform/iomap.h>
#include <Library/QcomTargetDisplayLib.h>

static struct fbcon_config config = {0};

RETURN_STATUS LibQcomTargetDisplayCallSecConstructors(VOID)
{
  return RETURN_SUCCESS;
}


static int mdp_dump_pipe_info(uint32_t *pipe_type, uint32_t *dual_pipe)
{
    int rc;

    *dual_pipe = 0;

    if (readl(MDP_VP_0_VIG_0_BASE + PIPE_SSPP_SRC0_ADDR)) {
        *pipe_type = MDSS_MDP_PIPE_TYPE_VIG;
        rc = 0;
    }
    else if (readl(MDP_VP_0_RGB_0_BASE + PIPE_SSPP_SRC0_ADDR)) {
        *pipe_type = MDSS_MDP_PIPE_TYPE_RGB;
        rc = 0;
    }
    else if (readl(MDP_VP_0_DMA_0_BASE + PIPE_SSPP_SRC0_ADDR)) {
        *pipe_type = MDSS_MDP_PIPE_TYPE_DMA;
        rc = 0;
    }
    else {
        rc = -1;
    }

    return rc;
}

static void mdp_select_pipe_type(uint32_t pipe_type, uint32_t *left_pipe, uint32_t *right_pipe)
{
    switch (pipe_type) {
        case MDSS_MDP_PIPE_TYPE_RGB:
            *left_pipe = MDP_VP_0_RGB_0_BASE;
            *right_pipe = MDP_VP_0_RGB_1_BASE;
            break;
        case MDSS_MDP_PIPE_TYPE_DMA:
            *left_pipe = MDP_VP_0_DMA_0_BASE;
            *right_pipe = MDP_VP_0_DMA_1_BASE;
            break;
        case MDSS_MDP_PIPE_TYPE_VIG:
        default:
            *left_pipe = MDP_VP_0_VIG_0_BASE;
            *right_pipe = MDP_VP_0_VIG_1_BASE;
            break;
    }
}

void target_display_init(void)
{
    int rc;
    uint32_t pipe_type;
    uint32_t dual_pipe;
    uint32_t left_pipe;
    uint32_t right_pipe;

    // dump pipe info
    rc = mdp_dump_pipe_info(&pipe_type, &dual_pipe);
    if(rc) {
        return;
    }

    // select pipes
    mdp_select_pipe_type(pipe_type, &left_pipe, &right_pipe);
    uint32_t pipe_base = left_pipe;

    // dump format
    config.base = (void *) readl(pipe_base + PIPE_SSPP_SRC0_ADDR);
    config.width = readl(pipe_base + PIPE_SSPP_SRC_YSTRIDE)/3;
    config.height = readl(pipe_base + PIPE_SSPP_SRC_IMG_SIZE)>>16;
    config.stride = config.width;
    config.bpp = 24;
    config.format = FB_FORMAT_RGB888;

    // setup fbcon
    fbcon_setup(&config);
}
