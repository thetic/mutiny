#!/bin/sh
# Semver-aware ABI compatibility check using abidiff.
#
# Usage: abi_check.sh <baseline_lib> <current_lib> <baseline_version> <current_version>
#
# abidiff exit code bitmask:
#   0  = no change
#   1  = abidiff error
#   2  = abidiff usage error
#   4  = ABI change (compatible, e.g. added symbols)
#   8  = ABI incompatible change (e.g. removed/changed symbols)
#
# Semver rules applied:
#   patch bump : no ABI changes allowed
#   minor bump : compatible additions allowed, incompatible changes not allowed
#   major bump : all ABI changes allowed (breaking changes are expected)

set -e

BASELINE_LIB=$1
CURRENT_LIB=$2
BASELINE_VERSION=$3
CURRENT_VERSION=$4

BASELINE_MAJOR=$(echo "$BASELINE_VERSION" | cut -d. -f1)
BASELINE_MINOR=$(echo "$BASELINE_VERSION" | cut -d. -f2)
CURRENT_MAJOR=$(echo "$CURRENT_VERSION" | cut -d. -f1)
CURRENT_MINOR=$(echo "$CURRENT_VERSION" | cut -d. -f2)

echo "Baseline version: $BASELINE_VERSION"
echo "Current version:  $CURRENT_VERSION"

ABIDIFF_EXIT=0
abidiff "$BASELINE_LIB" "$CURRENT_LIB" || ABIDIFF_EXIT=$?

HAS_CHANGE=$(( ABIDIFF_EXIT & 4 ))
HAS_INCOMPATIBLE=$(( ABIDIFF_EXIT & 8 ))
HAS_ERROR=$(( ABIDIFF_EXIT & ~12 ))

if [ "$HAS_ERROR" -ne 0 ]; then
    echo "abidiff encountered an error (exit code: $ABIDIFF_EXIT)"
    exit 1
fi

if [ "$HAS_CHANGE" -eq 0 ] && [ "$HAS_INCOMPATIBLE" -eq 0 ]; then
    echo "No ABI changes detected."
    exit 0
fi

if [ "$HAS_INCOMPATIBLE" -ne 0 ]; then
    if [ "$CURRENT_MAJOR" -gt "$BASELINE_MAJOR" ]; then
        echo "Major version bump ($BASELINE_VERSION -> $CURRENT_VERSION): incompatible ABI changes are allowed."
        exit 0
    fi
    echo "Incompatible ABI change detected without a major version bump."
    exit 1
fi

if [ "$CURRENT_MAJOR" -gt "$BASELINE_MAJOR" ] || [ "$CURRENT_MINOR" -gt "$BASELINE_MINOR" ]; then
    echo "Version bump ($BASELINE_VERSION -> $CURRENT_VERSION): compatible ABI changes are allowed."
    exit 0
fi

echo "Compatible ABI change detected without a minor version bump."
exit 1
