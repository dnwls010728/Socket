#pragma once

namespace RegistryHelper
{
    /**
     * \brief 레지스트리의 모든 값을 삭제합니다.
     */
    void DeleteAll();

    /**
     * \brief 키를 삭제합니다.
     * \param kKey 키
     */
    void DeleteKey(const std::wstring& kKey);

    /**
     * \brief 키가 존재하는지 확인합니다.
     * \param kKey 키
     * \return bool
     */
    bool HasKey(const std::wstring& kKey);

    /**
     * \brief int32_t 형의 값을 레지스트리에 저장합니다.
     * \param kKey 키
     * \param value 값
     */
    void SetInt(const std::wstring& kKey, int32_t value);

    /**
     * \brief float 형의 값을 레지스트리에 저장합니다.
     * \param kKey 키
     * \param value 값
     */
    void SetFloat(const std::wstring& kKey, float value);

    /**
     * \brief std::wstring 형의 값을 레지스트리에 저장합니다.
     * \param kKey 키
     * \param kValue 값
     */
    void SetString(const std::wstring& kKey, const std::wstring& kValue);

    /**
     * \brief int32_t 형의 값을 레지스트리에서 가져옵니다.
     * \param kKey 키
     * \param kDefaultValue 기본값
     * \return int32_t
     */
    int32_t GetInt(const std::wstring& kKey, int32_t kDefaultValue = 0);

    /**
     * \brief float 형의 값을 레지스트리에서 가져옵니다.
     * \param kKey 키
     * \param kDefaultValue 기본값
     * \return float
     */
    float GetFloat(const std::wstring& kKey, float kDefaultValue = 0.f);

    /**
     * \brief std::wstring 형의 값을 레지스트리에서 가져옵니다.
     * \param kKey 키
     * \param kDefaultValue 기본값
     * \return std::wstring
     */
    std::wstring GetString(const std::wstring& kKey, const std::wstring& kDefaultValue = L"");
};
