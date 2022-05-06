import sys
import re
file_name = sys.argv[1]

with open(file_name, 'r+') as file:
    header_content = file.read()
    regex_list = [r".*_attr_slot\(.*", r".*image_slot\(.*", 
            r".*uniformblock_slot\(.*", r".*uniformblock_size\(.*", r".*uniform_offset\(.*",
            r".*uniform_offset\(.*",r".*uniform_desc\(.*"]

    func_delcare_list = []
    for item in regex_list:
        func_delcare = re.search(item, header_content, re.M).group()
        func_delcare_list.append(func_delcare.strip(' {') + ";")

    func_delcare_str = "\n".join(func_delcare_list)

    regex = r"#endif\n(^.*_shader_desc\(.*;)"
    first_func_delcare = re.search(regex, header_content, re.M).group(1)

    func_delcare_str = "#endif\n" + first_func_delcare + "\n" +func_delcare_str

    regex = r"#endif\n^.*_shader_desc\(.*;"
    new_file_content = re.sub(regex, func_delcare_str, header_content, flags=re.M)
    print(new_file_content)
    file.write(new_file_content)


