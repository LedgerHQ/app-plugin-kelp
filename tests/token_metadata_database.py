class TokenMetadata:
	def __init__(self, ticker, address, decimals, chain_id):
		self.ticker = ticker
		self.address = address
		self.decimals = decimals
		self.chain_id = chain_id

STETH = TokenMetadata("stETH", "ae7ab96520de3a18e5e111b5eaab095312d7fe84", 18, 1)
ETHX = TokenMetadata("ETHx", "a35b1b31ce002fbf2058d22f30f95d405200a15b", 18, 1)

token_list = [STETH, ETHX]