import { DocumentType } from "@/types/DatabaseTypes";
import { ColumnDef } from "@tanstack/react-table";

const documents: DocumentType[] = [
  {
    time: "4:36",
    url: "https://youtube.com",
    path: "/jawn/youtube"
  },
  {
    time: "5:29",
    url: "https://wikipedia.com",
    path: "/info/wiki"
  },
  {
    time: "1:00",
    url: "https://foodnetwork.com",
    path: "/food/beef-wellington"
  },
]

const columns: ColumnDef<DocumentType>[] = [
  {
    accessorKey: "time",
    header: "Time"
  }
]

export default function Home() {
  return (
    <div className="h-screen flex flex-col justify-center items-center">
      
    </div>
  );
}
