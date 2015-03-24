

package trade;

public class FarmerRegister {
	
	private String Farmer_id;
	private String Farmer_region;
	private String Farmer_mobile;
	
	public String getId()
	{
		return this.Farmer_id;
	}
	public String getRegion()
	{
		return this.Farmer_region;
	}
	public String getMobile()
	{
		return this.Farmer_mobile;
	}
	
	public void setId(String id)
	{
		this.Farmer_id=id;
	}
	public void setRegion(String region)
	{
		this.Farmer_region=region;
	}
	public void setMobile(String mobile)
	{
		this.Farmer_mobile=mobile;
	}

}
