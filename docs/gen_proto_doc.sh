#!/bin/bash

# This script generates Markdown documentation from .proto file
# using https://github.com/pseudomuto/protoc-gen-doc
# and puts it in docs for GitHub Pages to read it from there.
#
# Uses a modified template to put services before messages in documentation.
#
# Requirements: docker.io installed and user added to docker group
#
# Run: ./gen_proto_doc.sh

out_extended="index_ext.md"
out="index.md"

proto="dictation_asr.proto"

# Pull a docker image.
docker pull pseudomuto/protoc-gen-doc

# Run the docker image.
docker run --rm \
  -v $(pwd):/out \
  -v $(pwd)/../submodules/googleapis/google:/google \
  -v $(pwd)/../proto:/protos \
  -v $(pwd):/templates \
  pseudomuto/protoc-gen-doc \
  -I/protos -I/ --doc_opt=/templates/markdownServiceFirst.tmpl,${out_extended} /protos/${proto}

# Manually truncate index.md to contain only API documentation (no included .proto's).
echo "# API Documentation"      > ${out}    # erase
echo "<a name="top"/>"          >> ${out}   # then append
echo ""                         >> ${out}
echo "### Table of Contents"    >> ${out}
echo ""                         >> ${out}

copy_line=false

table_of_contents_start="- [${proto}](#${proto})"
table_of_contents_end="- [Scalar Value Types](#scalar-value-types)"
content_start="<a name=\"${proto}\"/>"

while IFS='' read -r line || [[ -n "$line" ]]; do
    if [ "${line}" == "${table_of_contents_start}" ]; then
        copy_line=true
    fi
    if [ "${line}" == "${content_start}" ]; then
        copy_line=true
    fi

    ${copy_line} && echo "${line}" >> ${out}

    if [ "${line}" == "${table_of_contents_end}" ]; then
        echo "" >> ${out}
        echo "" >> ${out}
        echo "" >> ${out}
        copy_line=false
    fi
done < ${out_extended}

rm -f ${out_extended}
