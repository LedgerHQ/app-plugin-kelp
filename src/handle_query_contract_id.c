#include "plugin.h"

// Sets the first screen to display.
void handle_query_contract_id(ethQueryContractID_t *msg) {
    const context_t *context = (const context_t *) msg->pluginContext;
    // msg->name will be the upper sentence displayed on the screen.
    // msg->version will be the lower sentence displayed on the screen.

    // For the first screen, display the plugin name.
    strlcpy(msg->name, APPNAME, msg->nameLength);

    const char *msgVersion;

    // EDIT THIS: Adapt the cases by modifying the strings you pass to `strlcpy`.
    switch (context->selectorIndex) {
        case KELP_LST_DEPOSIT:
            msgVersion = "LST Restake";
            break;
        case KELP_ETH_DEPOSIT:
            msgVersion = "Native Restake";
            break;

        case KELP_INITIATE_WITHDRAW:
            msgVersion = "Unstake";
            break;

        case KELP_CLAIM_WITHDRAW:
            msgVersion = "Claim";
            break;

        case GAIN_DEPOSIT_ETH:
        case GAIN_DEPOSIT_LST:
        case GAIN_DEPOSIT_RSETH:
            msgVersion = "Gain Deposit";
            break;

        case GAIN_WITHDRAW:
            msgVersion = "Gain Withdraw";
            break;

        default:
            PRINTF("Selector index: %d not supported\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    strlcpy(msg->version, msgVersion, msg->versionLength);
    msg->result = ETH_PLUGIN_RESULT_OK;
}
