import { getRow } from "@/actions/DatabaseActions";
import { Dropdown } from "@/components/Dropdown";
const page = async () => {
    const data = await getRow()
    return (
        <div className="h-screen flex items-center justify-center">
            <Dropdown data={data}/>
        </div>
    )
}
export default page