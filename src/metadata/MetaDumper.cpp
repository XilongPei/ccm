//=========================================================================
// Copyright (C) 2018 The C++ Component Model(CCM) Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//=========================================================================

#include "MetaDumper.h"
#include "../ccdl/Parameter.h"
#include "../util/StringBuilder.h"

using ccm::ccdl::Parameter;

namespace ccm {
namespace metadata {

String MetaDumper::Dump()
{
    return DumpMetaComponent(mMetaComponet, String(""));
}

String MetaDumper::DumpMetaComponent(
    /* [in] */ MetaComponent* mc,
    /* [in] */ const String& prefix)
{
    StringBuilder builder;

    builder.Append(prefix).Append("MetaComponent\n");
    builder.Append(prefix).Append("{\n");
    builder.Append(prefix).AppendFormat("    mMagic:0x%x\n", mc->mMagic);
    builder.Append(prefix).AppendFormat("    mSize:%d\n", mc->mSize);
    builder.Append(prefix).Append("    mUuid:").Append(mc->mUuid.Dump()).Append("\n");
    builder.Append(prefix).Append("    mName:").Append(mc->mName).Append("\n");
    builder.Append(prefix).Append("    mUrl:").Append(mc->mUrl).Append("\n");
    builder.Append(prefix).AppendFormat("    mNamespaceNumber:%d\n", mc->mNamespaceNumber);
    builder.Append(prefix).AppendFormat("    mCoclassNumber:%d\n", mc->mCoclassNumber);
    builder.Append(prefix).AppendFormat("    mEnumerationNumber:%d\n", mc->mEnumerationNumber);
    builder.Append(prefix).AppendFormat("    mInterfaceNumber:%d\n", mc->mInterfaceNumber);
    builder.Append(prefix).AppendFormat("    mTypeNumber:%d\n", mc->mTypeNumber);
    builder.Append(prefix).Append("}\n");

    for (int i = 0; i < mc->mNamespaceNumber; i++) {
        String dumpNS = DumpMetaNamespace(mc->mNamespaces[i], prefix + "  ");
        builder.Append(dumpNS);
    }

    for (int i = 0; i < mc->mEnumerationNumber; i++) {
        String dumpEnum = DumpMetaEnumeration(mc->mEnumerations[i], prefix + "  ");
        builder.Append(dumpEnum);
    }

    for (int i = 0; i < mc->mInterfaceNumber; i++) {
        String dumpItf = DumpMetaInterface(mc->mInterfaces[i], prefix + "  ");
        builder.Append(dumpItf);
    }

    for (int i = 0; i < mc->mCoclassNumber; i++) {
        String dumpCls = DumpMetaCoclass(mc->mCoclasses[i], prefix + "  ");
        builder.Append(dumpCls);
    }

    return builder.ToString();
}

String MetaDumper::DumpMetaCoclass(
    /* [in] */ MetaCoclass* mc,
    /* [in] */ const String& prefix)
{
    StringBuilder builder;

    builder.Append(prefix).Append("MetaCoclass\n");
    builder.Append(prefix).Append("{\n");
    builder.Append(prefix).Append("    mName:").Append(mc->mName).Append("\n");
    builder.Append(prefix).Append("    mNamespace:").Append(mc->mNamespace).Append("\n");
    builder.Append(prefix).AppendFormat("    mInterfaceNumber:%d\n", mc->mInterfaceNumber);
    for (int i = 0; i < mc->mInterfaceNumber; i++) {
        builder.Append(prefix).AppendFormat("        %s\n",
                mMetaComponet->mInterfaces[mc->mInterfaceIndexes[i]]->mName);
    }
    builder.Append(prefix).Append("}\n");

    return builder.ToString();
}

String MetaDumper::DumpMetaConstant(
    /* [in] */ MetaConstant* mc,
    /* [in] */ const String& prefix)
{
    StringBuilder builder;

    builder.Append(prefix).Append("MetaConstant\n");
    builder.Append(prefix).Append("{\n");
    builder.Append(prefix).Append("    mName:").Append(mc->mName).Append("\n");
    builder.Append(prefix).Append("    mType:").Append(
            DumpMetaType(mMetaComponet->mTypes[mc->mTypeIndex])).Append("\n");
    builder.Append(prefix).Append("    mValue:").Append(
            DumpConstantValue(mc)).Append("\n");
    if (mc->mRadix != 0 ) {
        builder.Append(prefix).AppendFormat("    mRadix:%d\n", mc->mRadix);
    }
    builder.Append(prefix).Append("}\n");

    return builder.ToString();
}

String MetaDumper::DumpConstantValue(
    /* [in] */ MetaConstant* mc)
{
    StringBuilder builder;

    MetaType* mt = mMetaComponet->mTypes[mc->mTypeIndex];
    switch(mt->mKind) {
        case CcdlType::Char:
            builder.AppendFormat("%c", mc->mValue.mInteger);
            break;
        case CcdlType::Byte:
            builder.AppendFormat("%d", (unsigned char)mc->mValue.mInteger);
            break;
        case CcdlType::Short:
            builder.AppendFormat("%d", (short)mc->mValue.mInteger);
            break;
        case CcdlType::Integer:
            builder.AppendFormat("%d", mc->mValue.mInteger);
            break;
        case CcdlType::Long:
            builder.AppendFormat("%lld", mc->mValue.mLong);
            break;
        case CcdlType::Float:
            builder.AppendFormat("%f", mc->mValue.mFloat);
            break;
        case CcdlType::Double:
            builder.AppendFormat("%e", mc->mValue.mDouble);
            break;
        case CcdlType::Boolean:
            builder.Append(mc->mValue.mBoolean ? "true" : "false");
            break;
        case CcdlType::String:
            builder.Append(mc->mValue.mString);
            break;
        case CcdlType::Enum: {
            builder.Append(mc->mValue.mString);
            break;
        }
        default:
            break;
    }

    return builder.ToString();
}

String MetaDumper::DumpMetaEnumeration(
    /* [in] */ MetaEnumeration* me,
    /* [in] */ const String& prefix)
{
    StringBuilder builder;

    builder.Append(prefix).Append("MetaEnumeration\n");
    builder.Append(prefix).Append("{\n");
    builder.Append(prefix).Append("    mName:").Append(me->mName).Append("\n");
    builder.Append(prefix).Append("    mNamespace:").Append(me->mNamespace).Append("\n");
    builder.Append(prefix).AppendFormat("    mEnumeratorNumber:%d\n", me->mEnumeratorNumber);
    for (int i = 0; i < me->mEnumeratorNumber; i++) {
        builder.Append(prefix).AppendFormat("        %s = %d\n",
                me->mEnumerators[i]->mName, me->mEnumerators[i]->mValue);
    }
    builder.Append(prefix).Append("}\n");

    return builder.ToString();
}

String MetaDumper::DumpMetaInterface(
    /* [in] */ MetaInterface* mi,
    /* [in] */ const String& prefix)
{
    StringBuilder builder;

    builder.Append(prefix).Append("MetaInterface\n");
    builder.Append(prefix).Append("{\n");
    builder.Append(prefix).Append("    mName:").Append(mi->mName).Append("\n");
    builder.Append(prefix).Append("    mNamespace:").Append(mi->mNamespace).Append("\n");
    builder.Append(prefix).AppendFormat("    mConstantNumber:%d\n", mi->mConstantNumber);
    builder.Append(prefix).AppendFormat("    mMethodNumber:%d\n", mi->mMethodNumber);
    builder.Append(prefix).Append("}\n");

    for (int i = 0; i < mi->mConstantNumber; i++) {
        String dumpConst = DumpMetaConstant(mi->mConstants[i], prefix + "  ");
        builder.Append(dumpConst);
    }

    for (int i = 0; i < mi->mMethodNumber; i++) {
        String dumpMeth = DumpMetaMethod(mi->mMethods[i], prefix + "  ");
        builder.Append(dumpMeth);
    }

    return builder.ToString();
}

String MetaDumper::DumpMetaMethod(
    /* [in] */ MetaMethod* mm,
    /* [in] */ const String& prefix)
{
    StringBuilder builder;

    builder.Append(prefix).Append("MetaMethod\n");
    builder.Append(prefix).Append("{\n");
    builder.Append(prefix).Append("    mName:").Append(mm->mName).Append("\n");
    builder.Append(prefix).Append("    mSignature:").Append(mm->mSignature).Append("\n");
    builder.Append(prefix).AppendFormat("    mParameterNumber:%d\n", mm->mParameterNumber);
    for (int i = 0; i < mm->mParameterNumber; i++) {
        builder.Append(prefix).AppendFormat("        %s\n",
                DumpMetaParameter(mm->mParameters[i]).string());
    }
    builder.Append(prefix).Append("}\n");

    return builder.ToString();
}

String MetaDumper::DumpMetaNamespace(
    /* [in] */ MetaNamespace* mn,
    /* [in] */ const String& prefix)
{
    StringBuilder builder;

    builder.Append(prefix).Append("MetaNamespace\n");
    builder.Append(prefix).Append("{\n");
    builder.Append(prefix).Append("    mName:").Append(mn->mName).Append("\n");
    builder.Append(prefix).AppendFormat("    mCoclassNumber:%d\n", mn->mCoclassNumber);
    for (int i = 0; i < mn->mCoclassNumber; i++) {
        builder.Append(prefix).AppendFormat("        %s\n",
                mMetaComponet->mCoclasses[mn->mCoclassIndexes[i]]->mName);
    }
    builder.Append(prefix).AppendFormat("    mEnumerationNumber:%d\n", mn->mEnumerationNumber);
    for (int i = 0; i < mn->mEnumerationNumber; i++) {
        builder.Append(prefix).AppendFormat("        %s\n",
                mMetaComponet->mEnumerations[mn->mEnumerationIndexes[i]]->mName);
    }
    builder.Append(prefix).AppendFormat("    mInterfaceNumber:%d\n", mn->mInterfaceNumber);
    for (int i = 0; i < mn->mInterfaceNumber; i++) {
        builder.Append(prefix).AppendFormat("        %s\n",
                mMetaComponet->mInterfaces[mn->mInterfaceIndexes[i]]->mName);
    }
    builder.Append(prefix).Append("}\n");

    return builder.ToString();
}

String MetaDumper::DumpMetaParameter(
    /* [in] */ MetaParameter* mp)
{
    StringBuilder builder;

    builder.Append("mName:").Append(DumpMetaType(mMetaComponet->mTypes[mp->mTypeIndex]));
    builder.Append("[");
    bool needComma = false;
    if ((mp->mAttribute & Parameter::IN) != 0) {
        builder.Append("in");
        needComma = true;
    }
    if ((mp->mAttribute & Parameter::OUT) != 0) {
        if (needComma) builder.Append(",");
        builder.Append("out");
        needComma = true;
    }
    if ((mp->mAttribute & Parameter::CALLEE) != 0) {
        if (needComma) builder.Append(",");
        builder.Append("callee");
    }
    builder.Append("]");

    return builder.ToString();
}

String MetaDumper::DumpMetaType(
    /* [in] */ MetaType* mt)
{
    StringBuilder builder;

    switch(mt->mKind) {
        case CcdlType::Char:
            builder.Append("Char");
            break;
        case CcdlType::Byte:
            builder.Append("Byte");
            break;
        case CcdlType::Short:
            builder.Append("Short");
            break;
        case CcdlType::Integer:
            builder.Append("Integer");
            break;
        case CcdlType::Long:
            builder.Append("Long");
            break;
        case CcdlType::Float:
            builder.Append("Float");
            break;
        case CcdlType::Double:
            builder.Append("Double");
            break;
        case CcdlType::Boolean:
            builder.Append("Boolean");
            break;
        case CcdlType::String:
            builder.Append("String");
            break;
        case CcdlType::HANDLE:
            builder.Append("HANDLE");
            break;
        case CcdlType::Enum:
            builder.Append(mMetaComponet->mEnumerations[mt->mIndex]->mName);
            break;
        case CcdlType::Array:
            builder.Append("Array<").Append(
                    DumpMetaType(mMetaComponet->mTypes[mt->mNestedTypeIndex])).Append(">");
            break;
        case CcdlType::Interface:
            builder.Append(mMetaComponet->mInterfaces[mt->mIndex]->mName);
            break;
        default:
            break;
    }

    for (int i = 0; i < mt->mPointerNumber; i++) {
        builder.Append("*");
    }

    return builder.ToString();
}

}
}
