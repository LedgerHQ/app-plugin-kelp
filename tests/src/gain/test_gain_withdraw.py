from web3 import Web3
from tests.utils import run_test, load_contract

contract = load_contract(
    "0xe1B4d34E8754600962Cd944B535180Bd758E6c2e"
)

# Test from replayed transaction: https://etherscan.io/tx/0x3651c9a8214fc2342b22a26afddc6ec2fa7657f36a78543e328c19d8ded5e418

def test_gain_withdraw(backend, firmware, navigator, test_name, wallet_addr):
    data = "0x7d41c86e00000000000000000000000000000000000000000000000001246369f903c000000000000000000000000000057048197ff499d1f3111b05a1c6e9db9b27eea9000000000000000000000000057048197ff499d1f3111b05a1c6e9db9b27eea9"
    run_test(
        contract, 
        data, 
        backend, 
        firmware, 
        navigator, 
        test_name, 
        wallet_addr
    )

